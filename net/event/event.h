#ifndef SNET_EVENT_H
#define SNET_EVENT_H
#include <sys/epoll.h>
#include <callback.h>
#include <string>
#include <unordered_map>

// 前向声明
class TCPConnection;

class Event
{
public:
    typedef std::weak_ptr<TCPConnection> TCPConnWPtr;
    typedef std::shared_ptr<TCPConnection> TCPCoonSPtr;

    enum class OwnerType
    {
        NONE,           // 一般持有者，如单纯的事件循环            
        ACCEPTOR,       // 接收器
        CONNECTION,     // 连接
    };

    Event(OwnerType type = OwnerType::NONE, int fd = -1, uint32_t mask = 0);

    /// @brief 处理发生的事件
    void process();

    /// @brief 设置weak_ptr_
    /// @param conn TCP连接
    void set_weak_ptr(const TCPCoonSPtr &conn);

    /// @brief 设置读事件处理回调函数
    void set_read_callback(const CallBack& cb);

    /// @brief 设置写事件处理回调函数
    void set_write_callback(const CallBack& cb);

    /// @brief 设置关闭事件处理函数
    void set_close_callback(const CallBack &cb);

    /// @brief 关注事件中添加读事件
    void focus_read();

    /// @brief 关注事件中添加写事件
    void focus_write();

    /// @brief 关注RDHUP事件
    void focus_rdhup();

    /// @brief 关注PRI事件
    void focus_pri();

    /// @brief 关注ONESHOT事件
    void focus_oneshot();

    /// @brief 使用边沿触发模式
    void enable_et();

    /// @brief 取消关注读事件
    void unfocus_read();

    /// @brief 取消关注写事件
    void unfocus_write();

    /// @brief 取消关注RDHUP事件
    void unfocus_rdhup();

    /// @brief 取消关注PRI事件
    void unfocus_pri();

    /// @brief 取消关注ONESHOT事件
    void unfocus_oneshot();

    /// @brief 禁用边沿触发模式
    void disable_et();

    /// @brief 获取事件fd
    /// @return fd
    int fd()const;

    /// @brief 获取关注事件的掩码
    /// @return 掩码
    uint32_t mask()const;

    /// @brief 将掩码置为新值
    /// @param mask 新的掩码
    void set_mask(uint32_t mask);

    /// @brief 将事件掩码转化为字符串
    /// @param mask 事件掩码
    /// @return 字符串
    static std::string mask_to_string(uint32_t mask);

private:
    int             fd_;                // 关注的文件描述符
    uint32_t        mask_;              // 关注事件的掩码
    TCPConnWPtr     weak_ptr_;          // connection的weak_ptr，用于判断是否存活
    OwnerType       owner_;             // 所有者的类型，用于判断是否使用weak_ptr判断存活

    CallBack        read_callback_;     // 处理读事件的回调函数
    CallBack        write_callback_;    // 处理写事件的回调函数
    CallBack        close_callback_;    // 处理关闭事件的回调函数
};

#endif