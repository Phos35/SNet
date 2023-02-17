#ifndef SNET_TIMESTAMP_H
#define SNET_TIMESTAMP_H
#include <sys/time.h>
#include <time.h>
#include <string>

class Timestamp
{
public:
    Timestamp();

    /// @brief 获取当前时间的时间戳
    /// @return 时间戳
    static Timestamp now();

    /// @brief 格式化时间戳，转换为字符串
    /// @param format_str 转换的格式:%Y-Year, %m-month, %d-day, %H-hour,
    ///                   %M-minute, %S-second, %s-microsecond
    /// @return 格式化的时间字符串
    std::string format(const char *format_str);

    /// @brief 判断两个时间戳是否在同一天
    /// @param a 时间戳
    /// @param b 时间戳
    /// @return 若在同一天返回true，否则返回false
    static bool is_different_day(const Timestamp &a, const Timestamp &b);

private:
    timeval     current_tv_;
    tm*         current_tm_;

    char        last_str_[64];
};

#endif