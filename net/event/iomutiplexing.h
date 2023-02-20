#ifndef SNET_IOMUTIPLEXING_H
#define SNET_IOMUTIPLEXING_H
#include "event.h"
#include <vector>

class IOMutiplexing
{
public:
    /// @brief 获取活跃事件
    /// @param events 待填入活跃事件的数组
    /// @param timeout 监听等待的超时时间，单位ms；-1表示阻塞等待，0表示不等待
    /// @return 活跃事件的数量
    virtual size_t get_fired_events(std::vector<Event> &events, 
                                    int timeout = -1)=0;

    /// @brief 更新事件 -- 如关注的事件类型等
    /// @param e 待更新的事件
    virtual void update_event(Event &e);

    /// @brief 将事件加入监视
    /// @param e 待添加的事件
    virtual void add_event(Event &e);

    /// @brief 取消监视事件
    /// @param e 带取消的事件
    virtual void del_event(Event &e);

private:
};

#endif