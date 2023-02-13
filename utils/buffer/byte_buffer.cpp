#include "byte_buffer.h"
#include <string.h>

ByteBuffer::ByteBuffer(int capacity)
: read_index_(0), write_index_(0), capacity_(capacity),
  buffer_(capacity_, 0)
{
    
}

size_t ByteBuffer::write(const char* val, size_t length)
{
    // 待写入的内容长度大于可写入的长度，则不写入
    if(length > writeable())
        return 0;
    
    // 空间压缩
    if(length > post_writeable())
        compact();
    
    // 写入数据
    buffer_.replace(write_index_, length, val);
    write_index_ += length;

    return length;
}

size_t ByteBuffer::write(const std::string& val)
{
    return write(val.c_str(), val.size());
}

size_t ByteBuffer::read(char* dst, size_t length)
{
    // dst为空指针，则不进行写入
    if(dst == nullptr)
        return 0;

    memcpy(dst, buffer_.c_str() + read_index_, length);
    return length;
}

std::string ByteBuffer::read(size_t length)
{
    // 若length > readable() 或者 =0，则读取全部内容
    length = std::min(readable(), length);
    if(length == 0)
        length = readable();

    return buffer_.substr(read_index_, length);
}

std::string ByteBuffer::release(size_t length)
{
    std::string released = read(length);
    read_index_ += released.size();
    return released;
}

void ByteBuffer::compact()
{
    size_t content_len = readable();
    buffer_.replace(0, content_len, buffer_, read_index_, content_len);

    read_index_ = 0;
    write_index_ = content_len;
}

size_t ByteBuffer::readable()
{
    return write_index_ - read_index_;
}

size_t ByteBuffer::writeable()
{
    return pre_writeable() + post_writeable();
}

size_t ByteBuffer::pre_writeable()
{
    return read_index_;
}

size_t ByteBuffer::post_writeable()
{
    return capacity_ - write_index_;
}

size_t ByteBuffer::extend(size_t increment)
{
    buffer_.reserve(buffer_.size() + increment);
    capacity_ = buffer_.capacity();
    return buffer_.capacity();
}

std::string ByteBuffer::to_string()
{
    char info[256] = {0};
    snprintf(info, 256,
             "read_index_: %lu, write_index_: %lu,\n \
             \rreadable: %lu, writeable: %lu \n \
             \rcontent: %s\n\n",
             read_index_, write_index_, readable(), writeable(), buffer_.c_str());
    return info;
}