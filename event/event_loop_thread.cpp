#include <thread>

#include "event_loop_thread.h"
#include "logger.h"

EventLoopThread::EventLoopThread()
:id_(-1), event_loop_(nullptr),
 state_(State::IDLE)
{

}

EventLoopThread::~EventLoopThread()
{
    // 若事件循环所在线程还在运行，则退出线程
    if(state_ == State::RUNNING)
    {
        quit();
    }

    // 等待线程结束
    thread_->join();
    LOG_TRACE << "EventLoopThread " << id_ << ", thread terminate";
}

EventLoop* EventLoopThread::start()
{
    // 线程已经启动
    if(state_ == State::RUNNING)
    {
        LOG_WARN << "EventLoopThread " << id_ << " has already started";
        return event_loop_;
    }

    // 启动线程
    LOG_INFO << "EventLoopThread starting";
    thread_ = std::make_unique<std::thread>(&EventLoopThread::thread_func, this);

    // 等待线程创建事件循环
    {
        std::unique_lock<std::mutex> lck(mutex_);
        cond_.wait(lck, [this]()->bool
               { return event_loop_ != nullptr; });
    }
    id_ = event_loop_->id();
    
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

    // 启动事件循环
    state_ = State::RUNNING;
    LOG_INFO << "EventLoopThread started";
    loop.start();

    // 事件循环结束
    state_ = State::QUITTED;
    LOG_INFO << "EventLoopThread quitted";
}

/// @brief 退出事件循环线程
void EventLoopThread::quit()
{
    if(state_ == State::QUITTING)
    {
        LOG_WARN << "EventLoopThread " << id_ << " is still quitting";
        return;
    }

    if(state_ == State::QUITTED)
    {
        LOG_WARN << "EventLoopThread " << id_ << " has already quitted";
        return;
    }

    // 设置正在退出的标志，通知事件循环退出
    state_ = State::QUITTING;
    LOG_INFO << "EventLoopThread " << id_ << " quitting";
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