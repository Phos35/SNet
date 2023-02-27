#include "dispatcher.h"

Dispatcher::Dispatcher()
{

}

Dispatcher::~Dispatcher()
{

}

Message dispatch(Message* msg)
{
    // 基类dispatcher不做任何分发，返回原数据
    return *msg;
}