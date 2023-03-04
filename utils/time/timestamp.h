#ifndef SNET_TIMESTAMP_H
#define SNET_TIMESTAMP_H
#include <sys/time.h>
#include <time.h>
#include <string>

class Timestamp
{
public:
    Timestamp();
    Timestamp(suseconds_t microseconds);

    /// @brief 获取当前时间的时间戳
    /// @return 时间戳
    static Timestamp now();

    /// @brief 返回时间戳的秒数
    /// @return 时间戳的秒数
    time_t seconds();

    /// @brief 填充前导0
    /// @param str 代填充的字符串
    /// @param length 字符串长度
    /// @return 填充前导0到指定长度的字符串
    std::string zero_prefix(const std::string& str, int length);

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

    /// @brief 重载<运算符
    /// @param another 比较的时间戳
    /// @return 时间戳小于返回true，否则返回false
    bool operator<(const Timestamp &another);

    /// @brief 重载>运算符
    /// @param another 比较的时间戳
    /// @return 时间戳大于返回true，否则返回false
    bool operator>(const Timestamp &another);

    /// @brief 重载==运算符
    /// @param another 比较的时间戳
    /// @return 时间戳等于返回true，否则返回false
    bool operator==(const Timestamp &another);

    /// @brief 时间戳减法运算，按微秒数相减
    /// @param a 减数
    /// @param b 被减数
    /// @return 相减得到的时间戳
    friend Timestamp operator-(const Timestamp& a, const Timestamp &b);

private:
    timeval     current_tv_;

    time_t      year_;
    time_t      month_;
    time_t      day_;
    time_t      hour_;
    time_t      minute_;
    time_t      seconds_;
    time_t      micro_seconds_;

    /// @brief 将tv转成tm，再将tm指针中的数据读取到Timestamp中
    ///        采取这种做法是因为localtime返回的是静态指针
    ///        若将该指针作为成员，则数据就会出现错误
    void to_data();
};

#endif