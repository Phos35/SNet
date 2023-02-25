#include "acceptor.h"
#include <sys/types.h>
#include <sys/socket.h>
#include "logger.h"

Acceptor::Acceptor(int listen_fd)
:listen_fd_(listen_fd)
{

}

void Acceptor::start()
{
    // 启动事件循环线程
    EventLoop* loop = thread_.start();

    // 开始监听
    int ret = listen(listen_fd_, 10000);

    // 绑定监听fd上的读事件处理函数，并将事件注册到事件循环中
    Event listen_event(Event::OwnerType::ACCEPTOR, listen_fd_, EPOLLIN);
    listen_event.set_read_callback(std::bind(&Acceptor::accept, this));
    loop->add_event(listen_event);
}

void Acceptor::accept()
{
    sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = accept4(listen_fd_, (sockaddr*)&client_addr, &addr_len, SOCK_NONBLOCK);
    if(client_fd == -1)
    {
        // 发生错误
        if(errno != EAGAIN && errno != EWOULDBLOCK)
        {
            LOG_ERROR << "Acceptor Error " << strerror(errno);
        }

        // 无连接到来
        return;
    }

    // 处理新连接
    std::unique_ptr<Socket> soc = std::make_unique<Socket>(client_fd, &client_addr);
    conn_callback_(std::move(soc));
}

void Acceptor::set_conn_callback(const ConnCallBack& cb)
{
    conn_callback_ = std::move(cb);
}