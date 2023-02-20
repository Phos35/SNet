#include "event_loop_thread_pool.h"
#include "config.h"
#include "logger.h"

EventLoopThreadPool::EventLoopThreadPool(size_t size)
: running_(false), threads_cnt_(size)
{
    
}

EventLoopThreadPool::~EventLoopThreadPool()
{
    if(running_ == false)
        return;

    // 线程池仍在运行，则结束所有线程
    quit_all();
}

void EventLoopThreadPool::start()
{
    // 创建指定数量的线程
    for (int i = 0; i < threads_cnt_; i++)
    {
        threads_.push_back(std::make_unique<ELThread>());
        threads_[i]->start();
    }
    LOG_INFO << "EventLoopPool Size: " << threads_.size();
    running_ = true;
}

void EventLoopThreadPool::quit_all()
{
    /// EventLoopThread 析构时会退出事件循环
    /// 注意：EventLoopThread析构时join事件循环所在线程
    ///       因此可能会消耗较长时间
    while(threads_.empty() == false)
    {
        threads_.pop_back();
    }
}

EventLoop* EventLoopThreadPool::get_one()
{
    // 线程池尚未开始工作，没有事件循环被存储
    if(running_ == false)
        return nullptr;

    return balance_select();
}

EventLoop* EventLoopThreadPool::balance_select()
{
    static int next = 0;
    EventLoop *el = threads_[next]->event_loop();
    next = (next + 1) % threads_.size();
    return el;
}