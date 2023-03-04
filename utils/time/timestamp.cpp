#include "timestamp.h"
#include <cstring>
#include <cerrno>

Timestamp::Timestamp()
{
    int ret = gettimeofday(&current_tv_, nullptr);
    if(ret != 0)
    {
        printf("Get Time Error: %s\n", strerror(errno));
        return;
    }

    to_data();
}

Timestamp::Timestamp(time_t microseconds)
{
    current_tv_.tv_sec = microseconds / 1000;
    current_tv_.tv_usec = microseconds % 1000;

    to_data();
}

void Timestamp::to_data()
{
    tm* current_tm = localtime(&current_tv_.tv_sec);
    
    year_ = current_tm->tm_year + 1900;
    month_ = current_tm->tm_mon + 1;
    day_ = current_tm->tm_mday;
    hour_ = current_tm->tm_hour;
    minute_ = current_tm->tm_min;
    seconds_ = current_tm->tm_sec;
    micro_seconds_ = current_tv_.tv_usec;
}

Timestamp Timestamp::now()
{
    return Timestamp();
}

bool Timestamp::is_different_day(const Timestamp &a, 
                                 const Timestamp &b)
{   
    return a.year_ != b.year_ ||
           a.month_ != b.month_ ||
           a.day_ != b.day_;
}

std::string Timestamp::zero_prefix(const std::string& str, int length)
{
    // 长度符合条件，不需要填充前导0
    int zero_num = length - str.size();
    if(zero_num == 0)
        return str;
    
    // 填充前导0
    std::string ret(zero_num, '0');
    return ret + str;
}

bool Timestamp::operator<(const Timestamp &another)
{
    uint64_t this_time = current_tv_.tv_sec + current_tv_.tv_usec;
    uint64_t another_time = another.current_tv_.tv_sec + another.current_tv_.tv_usec;
    return this_time < another_time;
}

bool Timestamp::operator>(const Timestamp &another)
{
    uint64_t this_time = current_tv_.tv_sec + current_tv_.tv_usec;
    uint64_t another_time = another.current_tv_.tv_sec + another.current_tv_.tv_usec;
    return this_time < another_time;
}

bool Timestamp::operator==(const Timestamp &another)
{
    uint64_t this_time = current_tv_.tv_sec + current_tv_.tv_usec;
    uint64_t another_time = another.current_tv_.tv_sec + another.current_tv_.tv_usec;
    return this_time == another_time;
}

Timestamp operator-(const Timestamp& a, const Timestamp& b)
{
    suseconds_t a_micro = a.current_tv_.tv_sec * 1000000 + a.current_tv_.tv_usec;
    suseconds_t b_micro = b.current_tv_.tv_sec * 1000000 + b.current_tv_.tv_usec;
    printf("a: %ld, a.usec: %ld, b:%ld, b.usec = %ld\n", a_micro, a.current_tv_.tv_usec, b_micro, b.current_tv_.tv_usec);
    return Timestamp(a_micro - b_micro);
}

time_t Timestamp::seconds()
{
    return current_tv_.tv_sec;
}

// FIXME: 可能存在性能问题
std::string Timestamp::format(const char *format_str)
{
    std::string date;
    for (int i = 0; i < strlen(format_str); i++)
    {
        if(format_str[i] != '%')
        {
            date.push_back(format_str[i]);
            continue;
        }

        switch(format_str[i + 1])
        {
            case 'Y': date += zero_prefix(std::to_string(year_), 4); break;
            case 'm': date += zero_prefix(std::to_string(month_), 2); break;
            case 'd': date += zero_prefix(std::to_string(day_), 2); break;
            case 'H': date += zero_prefix(std::to_string(hour_), 2); break;
            case 'M': date += zero_prefix(std::to_string(minute_), 2); break;
            case 'S': date += zero_prefix(std::to_string(seconds_), 2); break;
            case 's': date += zero_prefix(std::to_string(micro_seconds_), 6); break;
            default: date += "UNKNOWN"; break;
        }
        ++i;
    }
    return date;
}