#include "event.h"
#include "logger.h"
#include "tcp_connection.h"

Event::Event(OwnerType type, int fd, uint32_t mask)
: owner_(type), fd_(fd), mask_(mask)
{

}

void Event::process()
{
    // LOG_TRACE << "fd " << fd_ << " happend events: " << mask_to_string(mask_);
    TCPConnection::SPtr ptr = weak_ptr_.lock();
    if(owner_ == OwnerType::CONNECTION && !ptr)
    {
        LOG_ERROR << "Connection has been destroyed";
        return;
    }

    // 读事件
    if(mask_ & EPOLLIN)
        if(!read_callback_)
            printf("fd %d read_callback not set!\n", fd_);
        else
            read_callback_();

    // 写事件
    if(mask_ & EPOLLOUT)
        if(!write_callback_)
            printf("fd %d write_callback not set!\n", fd_);
        else
            write_callback_();
    
    // 对端关闭写
    if(mask_ & EPOLLRDHUP)
        if(!close_callback_)
            printf("fd %d close_callback not set!\n", fd_);
        else
            close_callback_();
}

void Event::set_weak_ptr(const TCPCoonSPtr &conn)
{
    weak_ptr_ = conn;
}

void Event::set_read_callback(const CallBack& cb)
{
    read_callback_ = std::move(cb);
}

void Event::set_write_callback(const CallBack& cb)
{
    write_callback_ = std::move(cb);
}

void Event::set_close_callback(const CallBack &cb)
{
    close_callback_ = std::move(cb);
}

void Event::focus_read()
{
    mask_ |= EPOLLIN;
}

void Event::focus_write()
{
    mask_ |= EPOLLOUT;
}

void Event::focus_rdhup()
{
    mask_ |= EPOLLRDHUP;
}

void Event::focus_pri()
{
    mask_ |= EPOLLPRI;
}

void Event::focus_oneshot()
{
    mask_ |= EPOLLONESHOT;
}

void Event::enable_et()
{
    mask_ |= EPOLLET;
}

void Event::unfocus_read()
{
    mask_ &= ~EPOLLIN;
}

void Event::unfocus_write()
{
    mask_ &= ~EPOLLOUT;
}

void Event::unfocus_rdhup()
{
    mask_ &= ~EPOLLRDHUP;
}

void Event::unfocus_pri()
{
    mask_ &= ~EPOLLPRI;
}

void Event::unfocus_oneshot()
{
    mask_ &= ~EPOLLONESHOT;
}

void Event::disable_et()
{
    mask_ &= ~EPOLLET;
}

int Event::fd() const
{
    return fd_;
}

uint32_t Event::mask() const
{
    return mask_;
}

void Event::set_mask(uint32_t mask)
{
    mask_ |= mask;
}

std::string Event::mask_to_string(uint32_t mask)
{
    std::string events = "[";
    if (mask & EPOLLIN)
        events += "EPOLLIN,";
    if(mask & EPOLLOUT)
        events += "EPOLLOUT,";
    if(mask & EPOLLRDHUP)
        events += "EPOLLRDHUP,";
    if(mask & EPOLLPRI)
        events += "EPOLLPRI,";
    if(mask & EPOLLERR)
        events += "EPOLLERR,";
    if(mask & EPOLLHUP)
        events += "EPOLLHUP,";

    if(events.size() != 1)
        events.pop_back();

    events += "]";
    return events;
}