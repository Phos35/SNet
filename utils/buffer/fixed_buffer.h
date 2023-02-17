#ifndef SNET_FIXED_BUFFER_H
#define SNET_FIXED_BUFFER_H
#include <string>
#include <string.h>

// 小缓冲区大小 -- 4KB
#define SMALL_BUFFER_SIZE 4 * 1024
// 大缓冲区大小 -- 4MB
#define LARGE_BUFFER_SIZE SMALL_BUFFER_SIZE * 1024

template <int SIZE>
class FixedBuffer
{
public:
    FixedBuffer();

    /// @brief 向缓冲区中写入内容
    /// @param val 待写入的内容
    /// @param length 待写入内容的长度
    /// @return 实际写入的长度
    size_t write(const char *val, size_t length);

    /// @brief 向缓冲区中写入内容
    /// @param val 待写入的内容
    /// @return 实际写入的长度
    size_t write(const std::string &val);

    /// @brief 获取buffer_中的数据
    /// @return buffer_数组
    const char *data();

    /// @brief 获取缓冲区可写入空间的大小
    /// @return 缓冲区可写入的大小
    size_t available();

    /// @brief 获取缓冲区容量
    /// @return 缓冲区容量
    size_t capacity();

    /// @brief 获取缓冲区已写入数据的长度
    /// @return 已写入数据的长度
    size_t size();

    /// @brief 输出缓冲区信息
    /// @return 缓冲区具体信息的字符串
    std::string to_string();

private:
    char    buffer_[SIZE];  // 缓冲区

    size_t  capacity_;      // 缓冲区容量
    size_t  size_;          // 缓冲区写入内容的长度
};

template <int SIZE>
FixedBuffer<SIZE>::FixedBuffer()
:capacity_(SIZE), size_(0)
{
    memset(buffer_, 0, SIZE);
}

template <int SIZE>
size_t FixedBuffer<SIZE>::write(const char *val, size_t length)
{
    if(length > available())
        return 0;

    memcpy(buffer_ + size_, val, length);
    size_ += length;
    return length;
}

template <int SIZE>
size_t FixedBuffer<SIZE>::write(const std::string &val)
{
    return write(val.c_str(), val.length());
}

template <int SIZE>
const char* FixedBuffer<SIZE>::data()
{
    return buffer_;
}

template <int SIZE>
size_t FixedBuffer<SIZE>::available()
{
    return capacity_ - size_;
}

template <int SIZE>
size_t FixedBuffer<SIZE>::capacity()
{
    return capacity_;
}

template <int SIZE>
size_t FixedBuffer<SIZE>::size()
{
    return size_;
}

template <int SIZE>
std::string FixedBuffer<SIZE>::to_string()
{
    char buf[SIZE + 256] = {0};
    sprintf(buf,
            "size_: %lu, capacity: %lu, available: %lu\n\
            \rcontent: %s\n",
            size_, capacity_, available(), buffer_);
    return buf;
}
#endif