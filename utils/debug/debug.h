#ifndef SNET_DEBUG_H
#define SNET_DEBUG_H
#include <string>

namespace SNet
{
namespace DEBUG
{
    /// @brief 调试用函数，用于将报文转换成一行，便于日志打印
    /// @param str 待输出的内容
    /// @return 将换行符转换为字符串的字符串
    std::string one_line(const std::string &str);
}
}

#endif