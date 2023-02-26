#include "worker_pool.h"
#include "tcp_connection.h"
#include "message.h"
#include "logger.h"

WorkerPool::WorkerPool(size_t pool_size, size_t tasks_capacity)
:pool_size_(pool_size), tasks_capacity_(tasks_capacity)
{

}

WorkerPool::~WorkerPool()
{
    if(running_ == true)
    {
        quit();
    }
}

void WorkerPool::start()
{
    running_ = true;
    for (int i = 0; i < pool_size_; i++)
    {
        pool_.push_back(std::thread(&WorkerPool::worker_func, this));
    }
}

void WorkerPool::add_task(const TCPConnSPtr& task)
{
    {
        std::lock_guard<std::mutex> lck(mutex_);
        if(tasks_.size() >= tasks_capacity_)
            return;
        else
            tasks_.push_back(task);
    }
    cond_.notify_one();
}

void WorkerPool::worker_func()
{
    while(running_ == true)
    {
        // 等待被唤醒
        TCPConnWPtr task;
        {
            std::unique_lock<std::mutex> lck(mutex_);
            cond_.wait(lck, [this]()
                       { return tasks_.empty() == false || running_ == false; });

            // 拿取任务
            if(tasks_.empty() == false)
            {
                task = tasks_.front();
                tasks_.pop_front();
            }
        }

        // 若为退出唤醒，则直接退出
        if(running_ == false)
            break;

        // 提升查看是否存活
        TCPConnSPtr conn = task.lock();
        // 若消亡则无操作
        if(!conn) 
            continue;

        // 解析数据获取消息
        TCPBuffer &buffer = conn->recv_buffer_ref();
        Message::SPtr msg = conn->decode(buffer.read(0));

        // 若解析未完成，则需要等待完整报文
        if(msg->get_result() == Message::DeocdeResult::FAILURE)
        {
            continue;
        }

        // 释放缓冲区内容
        buffer.release(msg->raw_size());

        // 分发消息
        conn->dispatch(msg);
    }
}


void WorkerPool::quit()
{
    running_ = false;
    cond_.notify_all();
    for (int i = 0; i < pool_.size(); i++)
    {
        pool_[i].join();
    }
}