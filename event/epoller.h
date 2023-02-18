#ifndef SNET_EPOLLER_H
#define SNET_EPOLLER_H
#include "event.h"
#include "iomutiplexing.h"
#include <unordered_map>

#define EE_BUFFER_DEFAULT_SIZE 16
#define EE_BUFFER_MAX_SIZE 128

class EPoller : IOMutiplexing
{
public:
    EPoller();
    ~EPoller();

    /// @brief 获取活跃事件
    /// @param events 待填入活跃事件的数组
    /// @param timeout 监听等待的超时时间，单位ms；-1表示阻塞等待，0表示不等待
    /// @return 活跃事件的数量
    size_t get_fired_events(std::vector<Event> &events,
                            int timeout = -1) override;

    /// @brief 更新事件 -- 如关注的事件类型等
    /// @param e 待更新的事件
    void update_event(Event &e) override;

    /// @brief 将事件加入监视
    /// @param e 待添加的事件
    void add_event(Event &e) override;

    /// @brief 取消监视事件
    /// @param e 带取消的事件
    void del_event(Event &e) override;

    // epoll_ctl的操作类型
    enum class CTL_TYPE
    {
        ADD = EPOLL_CTL_ADD,
        MOD = EPOLL_CTL_MOD,
        DEL = EPOLL_CTL_DEL
    };

private:
    typedef std::unordered_map<int, Event> Map;
    typedef std::vector<epoll_event> EEBuffer;

    int epfd_;          // 代表 epoll 实例的 fd

    Map events_;        // 监听的事件集合，以事件fd作为key
    EEBuffer ees_;      // epoll_wait获取epoll_event的缓冲区

    /// @brief 同一对事件进行操作 -- 添加、更新、删除
    /// @param e 待操作的事件
    /// @param type 操作类型
    /// @return 操作成功返回0，失败返回-1
    int ctl(Event &e, CTL_TYPE type);
};

#endif