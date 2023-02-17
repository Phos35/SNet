#include "async_log.h"
#include "config.h"

AsyncLog::AsyncLog()
:current_(std::make_unique<AsyncLog::Buffer>()), 
 next_(std::make_unique<AsyncLog::Buffer>()),
 running_(true),
 back_thraed_(&AsyncLog::back_write, this)
{
    // 读取唤醒间隔的参数
    Config &config = Config::get_instance();
    wake_interval_ = config.log_wakeup_interval();

    // 脱离后端线程
    back_thraed_.detach();
}

AsyncLog::~AsyncLog()
{
    // 结束子线程的运行
    running_ = false;
    cond_.notify_one();
}

void AsyncLog::write(const char *val)
{
    std::unique_lock<std::mutex> lck(mutex_);

    // 若current_空间充足，则写入current_
    if(current_->available() > strlen(val))
    {
        current_->write(val);
        return;
    }

    // 若next_不为空，则置换current
    if(next_)
    {
        current_.swap(next_);
        buffers_.push_back(std::move(next_));
    }
    // 若next_为空，则申请新的Buffer
    else
    {
        current_.reset(new Buffer());
    }
    current_->write(val);
    
    // 唤醒后台线程
    cond_.notify_one();
}

void AsyncLog::back_write()
{
    BufferPtr backup_1 = std::make_unique<Buffer>();
    BufferPtr backup_2 = std::make_unique<Buffer>();
    BufferVec output_buffers;

    while(running_ == true)
    {
        // 等待被唤醒
        {
            std::unique_lock<std::mutex> lck(mutex_);
            cond_.wait_for(lck,std::chrono::seconds(wake_interval_));

            // 获取current_buffer并补充
            buffers_.push_back(std::move(current_));
            current_.swap(backup_1);

            // 若next已空，则进行补充
            if(!next_)
                next_.swap(backup_2);

            // 交换前后端缓冲区
            output_buffers.swap(buffers_);
        }

        // 将缓冲区中的内容写入文件中
        printf("outputs size: %d\n", output_buffers.size());
        for (BufferPtr &buffer : output_buffers)
        {
            output_file_.write(buffer->data());
            buffer->clear();
        }

        // 重置缓冲区数量
        int size = output_buffers.size();
        output_buffers.resize(std::min(size, 2));

        // 补充备用缓冲区
        if(!backup_1)
        {
            backup_1.swap(output_buffers.back());
            output_buffers.pop_back();
        }
        if(!backup_2)
        {
            backup_2.swap(output_buffers.back());
            output_buffers.pop_back();
        }
    }
}