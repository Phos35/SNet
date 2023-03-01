#ifndef SNET_TCP_SERVER_H
#define SNET_TCP_SERVER_H
#include "acceptor.h"
#include "worker_pool.h"
#include "tcp_connection.h"
#include "event_loop_thread_pool.h"
#include "message_processor_factory.h"
#include <unordered_map>

class TCPServer
{
public:
    TCPServer(const std::string &addr, uint16_t port);
    virtual ~TCPServer();

    /// @brief 启动服务器
    void run();

protected:
    /// @brief 创建消息处理器工厂，为派生类提供的接口
    virtual void create_factory();

    /// @brief 设置消息处理器工厂的指针
    /// @param factory_ 设置值
    void set_factory(MessageProcessorFactory::UPtr&& factory);

private:
    typedef std::vector<TCPConnection::SPtr> ConnMap;
    typedef std::unique_ptr<EventLoopThreadPool> EVLTPoolPtr;
    typedef std::unique_ptr<WorkerPool> WorkerPoolPtr;
    typedef MessageProcessorFactory MsgProcessFactory;

    Socket                  server_socket_; // 服务端socket

    Acceptor*               acceptor_;      // 连接接收器，MasterReactor
    EVLTPoolPtr             evlt_pool_;     // 事件循环线程池，SubReactors
    WorkerPoolPtr           worker_pool_;   // 承担解析报文工作的线程池

    MsgProcessFactory::UPtr factory_;       // 消息处理器工厂，用于为worker线程创建消息处理器

    ConnMap                 conns_;         // 存储现存的连接
    size_t                  next_id_;       // 分配给下一个连接的id

    /// @brief 新连接到来，接收并创建连接
    /// @param client_socket 客户端socket
    void create_new_conn(Socket::UPtr&& client_socket);

    /// @brief 连接关闭
    /// @param conn 待关闭的连接
    void close_conn(const TCPConnection::SPtr &conn);
};

#endif