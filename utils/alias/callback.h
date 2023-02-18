#ifndef SNET_ALIAS_H
#define SNET_ALIAS_H

#include <functional>

// 基础回调函数
typedef std::function<void()> BasicCallBack;

// 字符数组类型参数的回调函数
typedef std::function<void(const char *)> CharsCallBack;
typedef CharsCallBack OutputCallBack;

#endif