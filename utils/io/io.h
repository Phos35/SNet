#ifndef SNET_IO_H
#define SNET_IO_H
#include <string>
#include <iostream>
#include <sstream>

namespace SNet
{
/// @brief 简化IO操作的对象、函数
namespace IO
{
    /// @brief 将实现了<<重载的对象输出转换成string
    /// @param val 待转换的对象
    /// @return 对象字符串
    template <typename T>
    std::string to_string(const T &val)
    {
        std::ostringstream ss;
        ss << val;
        return ss.str();
    }
}
}

#endif