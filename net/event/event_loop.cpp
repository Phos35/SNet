#include <cassert>
#include <sys/eventfd.h>
#include <unistd.h>

#include "event_loop.h"
#include "config.h"
#include "epoller.h"
#include "logger.h"

// 线程局部变量，标识各个线程
thread_local pid_t thread_id_ = gettid();

EventLoop::EventLoop()
:id_(gettid()),
 state_(State::IDLE),
 wake_fd_(eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK))
{
    assert(wake_fd_ != -1);

    // 若非本线程运行，则报错结束
    assert(id_ == thread_id_);

    // 读取Config文件，构建对应类型的poller
    Config &config = Config::get_instance();
    std::string poller_type = config.event_iomutiplexing();
    // TODO 添加poll、select的IOMutiplexing
    if(poller_type == "epoll")
        poller_ = std::make_unique<EPoller>();
    
    // 注册wake_fd的事件
    Event wake_event(wake_fd_, EPOLLIN);
    wake_event.set_read_callback(std::bind(&EventLoop::read_wake_fd, this));
    poller_->add_event(wake_event);
}

EventLoop::~EventLoop()
{
    // 若事件循环未结束，则停止
    if(state_ == State::RUNNING)
        quit();
}

void EventLoop::start()
{
    assert_in_loop_thread();

    // 已经启动
    if(state_ == State::RUNNING)
        return;
    
    // 启动循环
    LOG_INFO << "EventLoop " << id_ << " starting";
    loop();
}

void EventLoop::loop()
{
    int time_out = -1;

    state_ = State::RUNNING;
    LOG_INFO << "EventLoop " << id_ << " started";
    while (state_ == State::RUNNING)
    {
        // TODO 根据functor的处理情况和定时器的情况设置timeout
        // 调用IOMutiplexing接口获取活跃事件
        int fired_cnt = poller_->get_fired_events(fired_, time_out);
    
        // 处理监听到的事件
        process_fired(fired_cnt);

        // 处理functor
        // 退出的标志在此处被设置
        process_functor();
    }
    state_ = State::QUITTED;
    LOG_INFO << "EventLoop " << id_ << " quitted";
}

void EventLoop::quit()
{
    if(state_ == State::QUITTING)
    {
        LOG_WARN << "EventLoop " << id_ << " still qutting";
        return;
    }

    if(state_ == State::QUITTED)
    {
        LOG_WARN << "EventLoop " << id_ << " has already quitted";
        return;
    }

    // 设置QUITTING标志
    state_ = State::QUITTING;
    LOG_INFO << "EventLoop " << id_ << " quitting";
    
    // 唤醒事件循环以结束
    wakeup();
}

bool EventLoop::in_loop_thread()
{
    if(thread_id_ == id_)
        return true;
    else
        return false;
}

void EventLoop::assert_in_loop_thread()
{
    assert(in_loop_thread() == true);
}

void EventLoop::add_event(Event &e)
{
    if(in_loop_thread() == true)
        poller_->add_event(e);
    else
        run_in_loop(std::bind(&EventLoop::add_event_in_loop, this, e));
}

void EventLoop::add_event_in_loop(Event &e)
{
    poller_->add_event(e);
}

void EventLoop::run_in_loop(const Functor &functor)
{
    // 若调用发生在本线程内，则直接执行
    if(in_loop_thread() == true)
    {
        functor();
    }
    // 跨线程调用，加入到functors中
    // 并唤醒事件循环
    else
    {
        add_functor(functor);
        wakeup();
    }
}

void EventLoop::add_functor(const Functor &functor)
{
    // 互斥添加
    std::lock_guard<std::mutex> lck(functor_mtx_);
    functors_.push_back(functor);
}

void EventLoop::process_fired(size_t fired_cnt)
{
    for (int i = 0; i < fired_cnt; i++)
    {
        fired_[i].process();
    }
    fired_.clear();
}

// TODO 优化处理跨线程调用造成的延迟问题
void EventLoop::process_functor()
{
    // 互斥获取functors_
    std::vector<Functor> processing_functors;
    {
        std::lock_guard<std::mutex> lck(functor_mtx_);
        processing_functors.swap(functors_);
        functors_.clear();
    }

    // 处理事件
    for (int i = 0; i < processing_functors.size(); i++)
    {
        processing_functors[i]();
    }
}

void EventLoop::wakeup()
{
    int ret = eventfd_write(wake_fd_, 1);
    if(ret == EINVAL)
    {
        LOG_ERROR << "Wakeup Failed";
    }
}

void EventLoop::read_wake_fd()
{
    eventfd_t counter = 0;
    int ret = eventfd_read(wake_fd_, &counter);
    if (ret == EINVAL)
    {
        LOG_ERROR << "Read wake_fd_ failed";
    }
}

pid_t EventLoop::id()
{
    return id_;
}

EventLoop::State EventLoop::state()
{
    return state_;
}

std::string EventLoop::state_to_string(State s)
{
    switch(s)
    {
        case State::IDLE: return "IDLE"; break;
        case State::RUNNING: return "RUNNING"; break;
        case State::QUITTING: return "QUITTING"; break;
        case State::QUITTED: return "QUITTED"; break;
        default: return "UNKNOWN EVENTLOOP STATE"; break;
    }
}