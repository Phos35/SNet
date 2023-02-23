#include "tcp_buffer.h"
#include "logger.h"
#include <sys/uio.h>
#include <cstring>

TCPBuffer::TCPBuffer(int capacity)
: ByteBuffer(capacity), 
  base_capacity_(capacity), lower_half_cnt_(0)
{
}

// TODO 添加伸缩机制，避免buffer无限扩张
size_t TCPBuffer::read_from_fd(int fd)
{
    // 压缩buffer
    compact();

    // 利用聚散IO读取fd中的内容
    char tmp_buffer[65536] = {0};
    iovec vecs[2] = {0};
    vecs[0] = {.iov_base = writeable_area(), writeable()};
    vecs[1] = {tmp_buffer, sizeof(tmp_buffer)};
    int read_size = readv(fd, vecs, 2);
    // 发生错误
    if(read_size == -1)
    {
        LOG_ERROR << "Buffer read from file error: " << strerror(errno);
        return 0;
    }

    // 若写入长度小于等于writeable，则更新write_index_
    if(read_size <= writeable())
    {
        increase_write_index(read_size);
        lower_half_cnt_++;
        
        // 多次写入数据量较少，则缩小空间
        if(lower_half_cnt_ > 4)
        {
            shrink(std::max(capacity() / 2, base_capacity_));
        }
    }
    // 写入长度大于writeable，则原有buffer已写满，需要扩容后将剩余部分写入
    else
    {
        extend_write(tmp_buffer, read_size - writeable());
    }

    return read_size;
}

void TCPBuffer::extend_write(char *data, int size)
{
    // 更新write_index_
    increase_write_index(writeable());

    // 扩容
    extend(size);

    // 写入
    write(data, size);
}
