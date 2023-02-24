#ifndef SNET_DISPATCHER_H
#define SNET_DISPATCHER_H
#include "message.h"
#include "tcp_connection.h"

using TCPConnPtr = TCPConnection::SPtr;

class Dispatcher
{
public:
    Dispatcher();
    virtual ~Dispatcher();

    /// @brief 将消息分配到对应的处理函数
    /// @param conn 消息发生的连接
    /// @param msg 待处理的消息
    virtual void dispatch(const TCPConnPtr& conn, const Message::Ptr &msg);

private:
};

#endif