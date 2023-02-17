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

// FIXME: 可能存在性能问题
std::string Timestamp::format(const char *format_str)
{
    std::string basic_format(format_str);
    std::string micorsec;
    auto pos = basic_format.find("%s");
    if (pos != std::string::npos)
    {
        basic_format = basic_format.substr(0, pos);
        micorsec = std::to_string(current_tv_.tv_usec);
    }

    char buf[64] = {0};
    strftime(buf, 64, basic_format.c_str(), current_tm_);
    return buf + micorsec;
}