#include "log_stream.h"

LogStream& LogStream::operator<<(const char *val)
{
    buffer_.write(val, strlen(val));
    return *this;
}

LogStream& LogStream::operator<<(const std::string& val)
{
    buffer_.write(val);
    return *this;
}

LogStream& LogStream::operator<<(bool val)
{
    if(val == true)
        return operator<<("true");
    else
        return operator<<("false");
}

LogStream& LogStream::operator<<(short val)
{
    return operator<<(std::to_string(val));
}

LogStream& LogStream::operator<<(unsigned short val)
{
    return operator<<(std::to_string(val));
}

LogStream& LogStream::operator<<(int val)
{
    return operator<<(std::to_string(val));
}

LogStream& LogStream::operator<<(unsigned int val)
{
    return operator<<(std::to_string(val));
}

LogStream& LogStream::operator<<(long val)
{
    return operator<<(std::to_string(val));
}

LogStream& LogStream::operator<<(unsigned long val)
{
    return operator<<(std::to_string(val));
}

LogStream& LogStream::operator<<(long long val)
{
    return operator<<(std::to_string(val));
}

LogStream& LogStream::operator<<(unsigned long long val)
{
    return operator<<(std::to_string(val));
}

LogStream& LogStream::operator<<(float val)
{
    return operator<<(std::to_string(val));
}

LogStream& LogStream::operator<<(double val)
{
    return operator<<(std::to_string(val));
}

const char* LogStream::data()
{
    return buffer_.data();
}