#include "tcp_connection.h"
#include "logger.h"
#include "config.h"
#include "acceptor.h"
#include "event_loop_thread_pool.h"
#include <unordered_map>
#include <iostream>

using TCPConnPtr = TCPConnection::TCPConnPtr;
using SocketPtr = std::unique_ptr<Socket>;

std::unordered_map<size_t, TCPConnPtr> m;
EventLoopThreadPool pool(4);
bool end = false;

void on_read(const TCPConnPtr& conn, const char* buf)
{
    printf("%s:%hu > %s\n", conn->addr().c_str(), conn->port(), buf);
    if(end == false)
        conn->write(buf, strlen(buf));
    else
        conn->close();
}

void on_close(const TCPConnPtr& conn)
{
    // 获取对应的指针
    TCPConnPtr last_ptr = conn;

    // 从map中移除
    m.erase(conn->id());

    // 调用destroy, 在事件循环中回收资源
    EventLoop *loop = conn->event_loop();
    loop->run_in_loop(std::bind(&TCPConnection::destroy, last_ptr));
}

void on_arrive(SocketPtr&& socket)
{
    static size_t id = 0;

    // 从事件循环线程池中选中一个事件循环
    EventLoop *loop = pool.get_one();

    // 创建TCPConnection并加入到map中
    TCPConnPtr tcp_conn = std::make_unique<TCPConnection>(id, loop, std::move(socket));
    tcp_conn->set_msg_callback(on_read);
    tcp_conn->set_close_callback(on_close);
    m.insert({id, tcp_conn});

    // 调用创建函数，使其在事件循环中完成初始化
    loop->run_in_loop(std::bind(&TCPConnection::create, tcp_conn));
}

int main()
{
    Config &config = Config::get_instance("/home/ubuntu/projects/web_server/SNet/config/config.json");

    /*模拟Server启动过程*/
    // 创建监听socket
    SocketPtr sock = std::make_unique<Socket>(AF_INET, SOCK_STREAM, 0);
    sock->set_reuse_addr();
    sock->bind("127.0.0.1", 9112);

    // 启动事件循环线程池
    pool.start();

    // 启动Acceptor
    Acceptor a(sock->fd());
    a.set_conn_callback(on_arrive);
    a.start();
    /*模拟Server启动过程*/

    std::string input;
    while (input.find("QUIT") == std::string::npos)
    {
        std::cin >> input;
    }

    // 测试析构函数能否正常结束各个线程

    return 0;
}