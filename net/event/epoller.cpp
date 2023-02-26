#include "epoller.h"
#include "logger.h"
#include <assert.h>
#include <unistd.h>

EPoller::EPoller()
: ees_(EE_BUFFER_DEFAULT_SIZE, {0})
{
    // 创建epoll实例
    epfd_ = epoll_create(5);
    assert(epfd_ != -1);
}

EPoller::~EPoller()
{
    close(epfd_);
}

size_t EPoller::get_fired_events(std::vector<Event> &fire_events, int timeout)
{
    int fired_cnt = epoll_wait(epfd_, ees_.data(), ees_.size(), timeout);
    // 发生错误，则报错返回0
    if(fired_cnt == -1)
    {
        LOG_ERROR << "epoll_wait faild: " << strerror(errno);
        return 0;
    }

    // 将活跃事件填入events中
    for (int i = 0; i < fired_cnt; i++)
    {
        Event &e = events_[ees_[i].data.fd];
        e.set_mask(ees_[i].events);
        fire_events.push_back(e);
    }
    memset(ees_.data(), 0, sizeof(epoll_event) * ees_.size());

    // 伸缩ees_
    if(fired_cnt == ees_.size())
    {
        ees_.resize(std::min(fired_cnt * 2, EE_BUFFER_MAX_SIZE));
    }
    else if(fired_cnt < ees_.size() / 2)
    {
        ees_.resize(std::max(static_cast<int>(ees_.size() / 2), 
                            EE_BUFFER_DEFAULT_SIZE));
    }

    return fired_cnt;
}

void EPoller::update_event(Event &e)
{
    // 无效的事件fd
    if(e.fd() == -1)
    {
        LOG_ERROR << "Invalid fd";
        return;
    }

    auto itr = events_.find(e.fd());
    // 若事件未添加到监听事件集合中，则给出警告
    if(itr == events_.end())
    {
        LOG_WARN << "Trying to modify a fd " << e.fd() << " which no in epoll";
        return;
    }

    // 更新事件
    ctl(e, CTL_TYPE::MOD);
}

void EPoller::add_event(Event &e)
{
    // 无效的事件fd
    if(e.fd() == -1)
    {
        LOG_ERROR << "Invalid fd";
        return;
    }

    // 添加事件
    if(ctl(e, CTL_TYPE::ADD) == 0)
    {
        events_[e.fd()] = e;
    }
}

void EPoller::del_event(Event &e)
{
    // 无效的事件fd
    if(e.fd() == -1)
    {
        LOG_ERROR << "Invalid fd";
        return;
    }

    auto itr = events_.find(e.fd());
    // 若待删除的事件不在关注事件集合中，则报错返回
    if(itr == events_.end())
    {
        LOG_ERROR << "Delete Nonexist event, fd: " << e.fd();
        return;
    }

    // 删除事件
    if(ctl(e, CTL_TYPE::DEL) == 0)
    { 
        events_.erase(e.fd());
    }
}

int EPoller::ctl(Event &e, EPoller::CTL_TYPE type)
{
    epoll_event ee = {
        .events = e.mask(),
        .data = {.fd = e.fd()}
    };

    int ret = epoll_ctl(epfd_, static_cast<int>(type), e.fd(), &ee);
    if(ret == -1)
    {
        LOG_ERROR << "epoll_ctl failed: " << strerror(errno);
    }

    return ret;
}