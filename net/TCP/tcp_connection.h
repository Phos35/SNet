#ifndef SNET_TCP_CONNECTION_H
#define SNET_TCP_CONNECTION_H
#include "event.h"
#include "socket.h"
#include "event_loop.h"

/*  TODO   */
// 1. 需要Buffer
// 2. 完善write逻辑
// 3. 需要Decoder
// 4. 其余查看cpp文件
// 5. 所有需要使用到TCPConnection的callback均应使用weak_ptr来代替，
//    否则作为成员的回调函数会提升shared_ptr的引用计数并且不会降下来


/// @brief TCP连接的抽象，采用shared_ptr管理生命周期
class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
public:
    typedef std::unique_ptr<Socket> SocketPtr;
    typedef std::shared_ptr<TCPConnection> SPtr;
    typedef std::function<void(const SPtr &, const char*)> MsgCallBack;
    typedef std::function<void(const SPtr &)> CloseCallBack;

    /// TCP状态枚举类
    enum class State
    {
        CREATING,           // 正在创建中 -- TCPServer创建，但尚未或者正在事件循环内初始化
        CONNECTING,         // 创建完成，处于连接状态 -- 在所属的事件循环内初始化完成
        CLOSING,            // 正在关闭 -- 处于正在释放相关资源的过程中
        CLOSED              // 关闭完成，释放完所有资源
    };

    /// @brief 根据接收的客户端socket创建TCP连接
    /// @param id 连接id
    /// @param event_loop TCP连接所属的事件循环
    /// @param client_socket 代表客户端的socket
    TCPConnection(size_t id, EventLoop* event_loop, SocketPtr&& client_socket);

    /// @brief 析构，释放相关资源
    virtual ~TCPConnection();

    /// @brief 创建连接的初始化函数，必须交给所属事件循环执行
    void create();

    /// @brief 设置消息到来的处理回调函数
    /// @param msg_Cb 参数为TCPConnPtr的回调函数
    void set_msg_callback(const MsgCallBack& msg_cb);

    /// @brief 设置处理连接关闭的回调函数
    /// @param close_cb 参数为TCPConnPtr的回调函数
    void set_close_callback(const CloseCallBack &close_cb);

    /// @brief 发送消息给对端
    /// @param data 待发送的数据
    void write(const std::string &data);

    /// @brief 发送消息给对端
    /// @param data 待发送的数据
    /// @param length 数据长度
    void write(const char *data, size_t length);

    /// @brief 调用处理关闭的回调函数，关闭连接
    void close();

    /// @brief 回收资源，关闭连接的最后一道工作
    ///        必须在事件循环中执行，可以认为是
    ///        close_in_loop
    void destroy();

    /// @brief 获取连接id
    /// @return 连接id
    size_t id();

    /// @brief 获取所属的事件循环
    /// @return 事件循环指针
    EventLoop *event_loop();

    /// @brief 获取客户端地址
    /// @return 客户端地址
    std::string addr();

    /// @brief 获取客户端端口
    /// @return 客户端端口
    uint16_t port();

    /// @brief 获取状态对应的字符串
    /// @param s 状态
    /// @return 状态字符串
    static std::string state_to_string(State s);

private:
    size_t          id_;                // 由上层赋予的连接id
    EventLoop*      loop_;              // 所属的事件循环
    SocketPtr       socket_;            // 代表客户端的socket
    Event           event_;             // TCP连接管理的事件

    State           state_;             // TCP连接的状态

    MsgCallBack     msg_callback_;      // 处理新消息的回调函数 TODO 应当改为Decoder调用
    CloseCallBack   close_callback_;    // 处理关闭事件的回调函数

    /// 处理读事件
    void process_read();

    /// @brief 处理写事件
    void process_write();

    /// 处理关闭事件
    void process_close();

    /// @brief 将写入操作转移到事件循环中完成
    /// @param data 待写入的数据
    /// @param length 数据长度
    void write_in_loop(const char* data, size_t length);
};

#endif