#ifndef SNET_ALIAS_H
#define SNET_ALIAS_H

#include <functional>
#include <memory>
#include "socket.h"

// 基础回调函数
typedef std::function<void()> CallBack;

// EventLoop中的跨线程调用
typedef CallBack Functor;

// 线程执行的任务函数
typedef CallBack Job;

// 处理新连接的回调函数
typedef std::function<void(std::unique_ptr<Socket> &&)> NewConnCallBack;

// 字符数组类型参数的回调函数
typedef std::function<void(const char *)> CharsCallBack;
typedef CharsCallBack OutputCallBack;

#endif