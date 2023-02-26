#include "tcp_server.h"
#include "config.h"
#include "logger.h"
#include <iostream>

TCPServer::TCPServer(const std::string &addr, uint16_t port)
: server_socket_(AF_INET, SOCK_STREAM, 0), next_id_(0),
  decoder_(nullptr), dispatcher_(nullptr)
{
    // 启用端口复用，绑定socket
    server_socket_.set_reuse_addr();
    server_socket_.bind(addr, port);
}

TCPServer::~TCPServer()
{
    delete acceptor_;
    delete decoder_;
    delete dispatcher_;
}

void TCPServer::run()
{
    // 读取参数，初始化两个线程池
    Config &config = Config::get_instance();
    size_t event_loop_pool_size = config.event_loop_thread_pool_size();
    size_t woker_pool_size = config.worker_thread_pool_size();
    size_t tasks_queue_capacity = config.tasks_queue_capacity();
    evlt_pool_ = std::make_unique<EventLoopThreadPool>(event_loop_pool_size);
    worker_pool_ = std::make_unique<WorkerPool>(woker_pool_size, 
                                                tasks_queue_capacity);

    // 创建Decoder和Dispatcher
    create_decoder();
    create_dispatcher();

    // 创建Acceptor并注册新连接建立回调函数
    acceptor_ = new Acceptor(server_socket_.fd());
    acceptor_->set_conn_callback(std::bind(&TCPServer::create_new_conn, this, std::placeholders::_1));

    // 启动事件循环线程池
    evlt_pool_->start();

    // 启动工作线程池
    worker_pool_->start();

    // 启动Acceptor
    acceptor_->start();

    // 等待输入退出指令
    std::string input;
    while(input.find("QUIT") == std::string::npos)
    {
        std::cin >> input;
    }
}

void TCPServer::create_new_conn(Socket::UPtr&& client_socket)
{
    // 从事件循环线程池中获取一个事件循环
    EventLoop *loop = evlt_pool_->get_one();

    // 创建新连接并注册关闭回调函数
    TCPConnection::SPtr conn = std::make_shared<TCPConnection>(
        next_id_, loop, std::move(client_socket), 
        decoder_, dispatcher_, worker_pool_.get());
    conn->set_close_callback(std::bind(&TCPServer::close_conn, this, 
                             std::placeholders::_1));

    // 添加连接至map中
    conns_.insert({next_id_, conn});
    next_id_++;

    // 调用初始化函数
    LOG_DEBUG << "TCPConnection " << conn->id() << " accepted, counter: " << conn.use_count();
    loop->run_in_loop(std::bind(&TCPConnection::create, conn));
}

void TCPServer::close_conn(const TCPConnection::SPtr &conn)
{
    LOG_DEBUG << "Sever Before Erase, TCPConnection " << conn->id() << ", counter: " << conn.use_count();
    // 移除map中的连接
    conns_.erase(conn->id());

    // 调用销毁函数
    EventLoop *loop = conn->event_loop();
    loop->run_in_loop(std::bind(&TCPConnection::destroy, conn));
}

void TCPServer::create_decoder()
{
    set_decoder(new Decoder());
}

void TCPServer::create_dispatcher()
{
    set_dispatcher(new Dispatcher());
}

void TCPServer::set_decoder(Decoder::Ptr decoder)
{
    decoder_ = decoder;
}

void TCPServer::set_dispatcher(Dispatcher::Ptr dispatcher)
{
    dispatcher_ = dispatcher;
}