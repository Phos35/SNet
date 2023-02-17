#ifndef SNET_LOG_STREAM_H
#define SNET_LOG_STREAM_H

#include "fixed_buffer.h"

class LogStream
{
public:
    LogStream &operator<<(const char *val);
    LogStream &operator<<(const std::string& val);

    LogStream &operator<<(bool val);

    LogStream &operator<<(short val);
    LogStream &operator<<(unsigned short val);
    LogStream &operator<<(int val);
    LogStream &operator<<(unsigned int val);
    LogStream &operator<<(long val);
    LogStream &operator<<(unsigned long val);
    LogStream &operator<<(long long val);
    LogStream &operator<<(unsigned long long val);

    LogStream &operator<<(float val);
    LogStream &operator<<(double val);

    /// @brief 获取缓冲区数据
    /// @return 缓冲区的指针
    const char* data();

private:
    typedef FixedBuffer<SMALL_BUFFER_SIZE> Buffer;
    Buffer buffer_;     // 4KB缓冲区
};

#endif