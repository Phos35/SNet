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

private:
    timeval     current_tv_;
    tm*         current_tm_;

    char        last_str_[64];
};

#endif