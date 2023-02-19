#include <thread>

#include "event_loop_thread.h"
#include "logger.h"

EventLoopThread::EventLoopThread()
:id_(-1), event_loop_(nullptr),
 running_(false)
{

}

EventLoopThread::~EventLoopThread()
{
    // 若事件循环所在线程还在运行，则退出线程
    if(running_ == true)
    {
        quit();

        // 等待线程退出
        {
            std::unique_lock<std::mutex> lck(mutex_);
            cond_.wait(lck);
        }
    }
}

EventLoop* EventLoopThread::start()
{
    // 线程已经启动
    if(running_ == true)
    {
        LOG_WARN << "EventLoopThread " << id_ << " has already started";
        return event_loop_;
    }

    // 启动线程
    LOG_INFO << "EventLoopThread Starting";
    std::thread th(&EventLoopThread::thread_func, this);
    th.detach();

    // 等待线程创建事件循环
    {
        std::unique_lock<std::mutex> lck(mutex_);
        cond_.wait(lck, [this]()->bool
               { return event_loop_ != nullptr; });
        printf("Wait over\n");
    }

    id_ = event_loop_->id();
    running_ = true;

    return event_loop_;
}

void EventLoopThread::thread_func()
{
    // 创建事件循环，互斥赋值指针并通知事件循环已创建完毕
    EventLoop loop;
    {
        std::lock_guard<std::mutex> lck(mutex_);
        event_loop_ = &loop;
    }

    /// 此处notify必须放置在锁范围外
    /// notify在锁范围内时，消费者线程被唤醒后
    /// 尝试获取锁，而此时生产者可能仍处于锁的
    /// 范围中（因为调度），于是消费者获取锁失
    /// 败，这样就可能会造成预期外的行为
    cond_.notify_one();
    printf("Notify\n");
    LOG_INFO << "EventLoopThread Started";

    // 启动事件循环
    loop.start();

    // 告知线程退出
    LOG_INFO << "EventLoopThread " << id_ << " Quitted";
    cond_.notify_one();
}

/// @brief 退出事件循环线程
void EventLoopThread::quit()
{
    if(running_ == false)
    {
        LOG_WARN << "EventLoopThread " << id_ << " has already quitted";
        return;
    }

    LOG_INFO << "EventLoopThread " << id_ << " Quitting";
    event_loop_->quit();
}

/// @brief 获取运行事件循环的线程的id
/// @return 线程id
pid_t EventLoopThread::id()
{
    return id_;
}

/// @brief 获取线程正在运行的事件循环
/// @return 事件循环
EventLoop* EventLoopThread::event_loop()
{
    return event_loop_;
}