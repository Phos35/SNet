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

    current_tm_ = localtime(&current_tv_.tv_sec);
}

Timestamp Timestamp::now()
{
    return Timestamp();
}

bool Timestamp::is_different_day(const Timestamp &a, 
                                 const Timestamp &b)
{
    return a.current_tm_->tm_year != b.current_tm_->tm_year ||
           a.current_tm_->tm_mon != b.current_tm_->tm_mon ||
           a.current_tm_->tm_mday != b.current_tm_->tm_mday;
}

// FIXME: 可能存在性能问题
std::string Timestamp::format(const char *format_str)
{
    std::string basic_format(format_str);
    char micorsec[8] = {0};
    auto pos = basic_format.find("%s");
    if (pos != std::string::npos)
    {
        snprintf(micorsec, 8, "%06ld", current_tv_.tv_usec);
        basic_format = basic_format.substr(0, pos);
    }

    char buf[64] = {0};
    strftime(buf, 64, basic_format.c_str(), current_tm_);
    strncat(buf, micorsec, 8);
    return buf;
}