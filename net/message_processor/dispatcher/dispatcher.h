#ifndef SNET_DISPATCHER_H
#define SNET_DISPATCHER_H
#include "message.h"

class Dispatcher
{
public:
    typedef std::unique_ptr<Dispatcher> UPtr;

    Dispatcher();
    virtual ~Dispatcher();

    /// @brief 将消息分配到对应的处理函数
    /// @param msg 待处理的消息
    /// @return 上层的响应消息
    virtual Message dispatch(Message* msg);

private:
};

#endif