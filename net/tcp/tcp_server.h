#ifndef SNET_TCP_SERVER_H
#define SNET_TCP_SERVER_H
#include "acceptor.h"
#include "worker_pool.h"
#include "tcp_connection.h"
#include "event_loop_thread_pool.h"
#include <unordered_map>

class TCPServer
{
public:
    TCPServer(const std::string &addr, uint16_t port);
    virtual ~TCPServer();

    /// @brief 启动服务器
    void run();

protected:
    /// @brief 创建Decoder，留给子类的接口，结合set_decoder能够自定义decoder
    virtual void create_decoder();

    /// @brief 创建Dispatcher，留给子类的接口，结合set_dispatcher能够自定义dispatcher
    virtual void create_dispatcher();

    /// @brief 设置Decoder
    /// @param decoder decoder指针
    void set_decoder(Decoder::Ptr decoder);

    /// @brief 设置Dispatcher
    /// @param Dispatcher dispatcher指针
    void set_dispatcher(Dispatcher::Ptr dispatcher);

private:
    typedef std::unordered_map<size_t, TCPConnection::SPtr> ConnMap;
    typedef std::unique_ptr<EventLoopThreadPool> EVLTPoolPtr;
    typedef std::unique_ptr<WorkerPool> WorkerPoolPtr;

    Socket                  server_socket_; // 服务端socket

    Acceptor*               acceptor_;      // 连接接收器，MasterReactor
    EVLTPoolPtr             evlt_pool_;     // 事件循环线程池，SubReactors
    WorkerPoolPtr           worker_pool_;   // 承担解析报文工作的线程池             
    
    Decoder*                decoder_;       // 报文解析器
    Dispatcher*             dispatcher_;    // 消息分发器

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