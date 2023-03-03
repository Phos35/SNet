#include "tcp_server.h"
#include "config.h"
#include "logger.h"
#include <iostream>
#include <signal.h>

TCPServer::TCPServer(const std::string &addr, uint16_t port)
: server_socket_(AF_INET, SOCK_STREAM, 0), next_id_(0),
  conns_(100000), factory_(nullptr)
{
    // 启用端口复用，绑定socket
    server_socket_.set_reuse_addr();
    server_socket_.bind(addr, port);
}

TCPServer::~TCPServer()
{
    delete acceptor_;
}

void TCPServer::run()
{
    // 忽略sigpipe信号
    signal(SIGPIPE, SIG_IGN);

    // 读取参数，初始化两个线程池
    Config &config = Config::get_instance();
    size_t event_loop_pool_size = config.event_loop_thread_pool_size();
    size_t woker_pool_size = config.worker_thread_pool_size();
    size_t tasks_queue_capacity = config.tasks_queue_capacity();
    evlt_pool_ = std::make_unique<EventLoopThreadPool>(event_loop_pool_size);
    worker_pool_ = std::make_unique<WorkerPool>(woker_pool_size, 
                                                tasks_queue_capacity);

    // 创建消息处理器工厂
    create_factory();

    // 为线程池设置消息处理器工厂，并创建消息处理器
    worker_pool_->create_message_processor(std::move(factory_));

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
        if(input == "COUNT")
        {
            for (int i = 0; i < 100000; i++)
            {
                long use_cnt = conns_[i].use_count();
                if (use_cnt != 0)
                    printf("Connection %d, use_count: %ld\n", i, use_cnt);
            }
        }
    }
}

void TCPServer::create_new_conn(Socket::UPtr&& client_socket)
{
    // 从事件循环线程池中获取一个事件循环
    EventLoop *loop = evlt_pool_->get_one();

    // 创建新连接并注册关闭回调函数
    TCPConnection::SPtr conn = std::make_shared<TCPConnection>(
        next_id_, loop, std::move(client_socket), worker_pool_.get());
    conn->set_close_callback(std::bind(&TCPServer::close_conn, this, 
                             std::placeholders::_1));

    // 添加连接至map中
    conns_[next_id_] = conn;
    next_id_ = (next_id_ + 1) % 100000;

    // 调用初始化函数
    LOG_DEBUG << "TCPConnection " << conn->id() << " accepted, counter: " << conn.use_count();
    loop->run_in_loop(std::bind(&TCPConnection::create, conn));
}

void TCPServer::close_conn(const TCPConnection::SPtr &conn)
{
    LOG_DEBUG << "Sever Before Erase, TCPConnection " << conn->id() << ", counter: " << conn.use_count();
    // 移除map中的连接
    conns_[conn->id()] = nullptr;

    // 调用销毁函数
    EventLoop *loop = conn->event_loop();
    loop->run_in_loop(std::bind(&TCPConnection::destroy, conn));
}

void TCPServer::create_factory()
{
    set_factory(std::make_unique<MessageProcessorFactory>());
}

void TCPServer::set_factory(MessageProcessorFactory::UPtr&& factory)
{
    factory_ = std::move(factory);
}