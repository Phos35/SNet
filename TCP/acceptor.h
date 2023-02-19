#ifndef SNET_ACCEPTOR_H
#define SNET_ACCEPTOR_H
#include "event_loop_thread.h"
#include "callback.h"

class Acceptor
{
public:
    Acceptor(int listen_fd);

    /// @brief 启动监听
    void start();

    /// @brief 设置新连接到来时的处理回调函数
    void set_new_conn_callback(const NewConnCallBack& cb);

private:
    int             listen_fd_;         // 监听的文件描述符
    EventLoopThread thread_;            // 执行监听任务的事件循环线程

    NewConnCallBack new_conn_callback_; // 新连接到来时的回调处理函数

    /// @brief 接受新的连接
    void accept();
};

#endif