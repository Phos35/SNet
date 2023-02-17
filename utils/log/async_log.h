#ifndef SNET_ASYNC_LOG_H
#define SNET_ASYNC_LOG_H
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>

#include "log_file.h"
#include "fixed_buffer.h"

class AsyncLog
{
public:
    AsyncLog();
    ~AsyncLog();

    /// @brief 前端接口，向缓冲区写入数据
    /// @param val 待写入的数据
    void write(const char *val);

private:
    typedef FixedBuffer<LARGE_BUFFER_SIZE> Buffer;
    typedef std::unique_ptr<Buffer> BufferPtr;
    typedef std::vector<BufferPtr> BufferVec;

    LogFile                 output_file_;   // 日志输出文件

    BufferPtr               current_;       // 前端缓冲区1
    BufferPtr               next_;          // 前端缓冲区2
    BufferVec               buffers_;       // 前端缓冲区数组

    std::mutex              mutex_;         // 缓冲区互斥锁
    std::condition_variable cond_;          // 唤醒后端线程的条件变量

    bool                    running_;       // 后端线程运行标识
    std::thread             back_thraed_;   // 后端线程句柄

    size_t                  wake_interval_; // 定期唤醒的间隔
    
    /// @brief 后端线程，将缓冲区的内容写入文件中
    void back_write();
};

#endif