#include "event.h"

Event::Event(int fd, uint32_t mask)
:fd_(fd), mask_(mask)
{

}

void Event::process()
{
    // 读事件
    if(mask_ & EPOLLIN)
        read_callback_();

    // 写事件
    if(mask_ & EPOLLOUT)
        write_callback_();
}

void Event::set_read_callback(const BasicCallBack& cb)
{
    read_callback_ = std::move(cb);
}

void Event::set_write_callback(const BasicCallBack& cb)
{
    read_callback_ = std::move(cb);
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

int Event::fd()
{
    return fd_;
}

uint32_t Event::mask()
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