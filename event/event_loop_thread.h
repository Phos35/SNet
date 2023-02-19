#ifndef SNET_EVENT_LOOP_THREAD
#define SNET_EVENT_LOOP_THREAD
#include <mutex>
#include <condition_variable>
#include <pthread.h>
#include <sys/types.h>

#include "event_loop.h"

class EventLoopThread
{
public:
    EventLoopThread();
    ~EventLoopThread();

    /// @brief 启动一个运行循环的线程
    /// @return 被启动的事件循环
    EventLoop *start();

    /// @brief 退出事件循环线程
    void quit();

    /// @brief 获取运行事件循环的线程的id
    /// @return 线程id
    pid_t id();

    /// @brief 获取线程正在运行的事件循环
    /// @return 事件循环
    EventLoop *event_loop();

private:
    pid_t                   id_;            // 线程id
    EventLoop*              event_loop_;    // 运行的事件循环指针

    bool                    running_;       // 线程运行标志
    std::mutex              mutex_;         // 线程启动时获取event_loop的互斥锁
    std::condition_variable cond_;          // 告知event_loop已创建的条件变量

    /// @brief 线程函数，用于启动一个事件循环
    void thread_func();
};

#endif