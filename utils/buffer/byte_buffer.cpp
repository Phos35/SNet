#include "byte_buffer.h"
#include <string.h>

ByteBuffer::ByteBuffer(int capacity)
: read_index_(0), write_index_(0), 
  capacity_(capacity), base_capacity_(capacity),
  buffer_(capacity, 0)
{
    
}

char* ByteBuffer::writeable_area()
{
    return buffer_.data() + write_index_;
}

char* ByteBuffer::readable_area()
{
    return buffer_.data() + read_index_;
}

size_t ByteBuffer::write(const char* val, size_t length)
{
    // 空间压缩
    if(length > post_writeable() && pre_writeable() != 0)
        compact();

    // 待写入的内容长度大于可写入的长度，则不写入
    if(length > writeable())
        extend(length - writeable());
    // 写入内容过少，则压缩空间
    else if(length < writeable() / 4)
        shrink(std::max(writeable() / 4, base_capacity_));

    // 写入数据
    char *data = buffer_.data() + write_index_;
    memcpy(data, val, length);
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

    memcpy(dst, buffer_.data() + read_index_, length);
    return length;
}

std::string ByteBuffer::read(size_t length)
{
    // 若length > readable() 或者 =0，则读取全部内容
    length = std::min(readable(), length);
    if(length == 0)
        length = readable();

    return std::string(buffer_.data() + read_index_, length);
}

void ByteBuffer::release(size_t length)
{
    read_index_ += length;
}

void ByteBuffer::compact()
{
    // WARN: 注意此处的自拷贝，可能会导致问题
    size_t content_len = readable();
    memcpy(buffer_.data(), buffer_.data() + read_index_, content_len);

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

size_t ByteBuffer::capacity()
{
    return capacity_;
}

size_t ByteBuffer::base_capacity()
{
    return base_capacity_;
}

size_t ByteBuffer::extend(size_t increment)
{
    // WARN 此处不能使用reserve，原因可能与reserve中重新分配内存与size
    //      有关，后续需要深入
    buffer_.resize(capacity_ + increment);
    capacity_ = buffer_.size();
    return buffer_.size();
}

size_t ByteBuffer::shrink(size_t decrement)
{
    if(capacity_ <= decrement)
        return capacity_;

    capacity_ -= decrement;
    if(write_index_ < capacity_)
        write_index_ = capacity_;

    buffer_.resize(capacity_);
    return buffer_.size();
}

void ByteBuffer::increase_write_index(size_t increment)
{
    write_index_ += increment;
}

std::string ByteBuffer::to_string()
{
    char info[256] = {0};
    snprintf(info, 256,
             "capacity: %lu\n \
             \rread_index_: %lu, write_index_: %lu,\n \
             \rreadable: %lu, writeable: %lu \n \
             \rcontent: %s\n\n",
             capacity_, read_index_, write_index_, readable(), writeable(), std::string(buffer_.data() + read_index_, readable()).c_str());
    return info;
}