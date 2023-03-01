#ifndef SNET_WORKER_POOL_H
#define SNET_WORKER_POOL_H
#include <thread>
#include <vector>
#include <deque>
#include <mutex>
#include <condition_variable>
#include "message_processor_factory.h"

class TCPConnection;
using TCPConnWPtr = std::weak_ptr<TCPConnection>;
using TCPConnSPtr = std::shared_ptr<TCPConnection>;

class WorkerPool
{
public:
    WorkerPool(size_t pool_size, size_t tasks_capacity);
    ~WorkerPool();

    /// @brief 启动线程池
    void start();

    /// @brief 添加任务
    void add_task(const TCPConnSPtr& task);

    /// @brief 利用消息处理器工厂创建消息处理器
    /// @param factory 消息处理器工厂
    void create_message_processor(MessageProcessorFactory::UPtr&& factory);

private:
    std::deque<TCPConnWPtr>         tasks_;             // 任务，实质为TCPConnection的weak_ptr
    std::vector<std::thread>        pool_;              // 线程池
    std::mutex                      mutex_;             // 访问任务队列的互斥量
    std::condition_variable         cond_;              // 唤醒工作线程的条件变量
    bool                            running_;           // 运行标志

    size_t                          tasks_capacity_;    // 任务队列容量
    size_t                          pool_size_;         // 线程数量

    MessageProcessorFactory::UPtr   factory_;           // 消息处理器工厂

    /// @brief 工作线程函数
    void worker_func(MessageProcessor::UPtr&& processor);

    /// @brief 结束所有线程
    void quit();
};

#endif