#ifndef SNET_BYTE_BUFFER_H
#define SNET_BYTE_BUFFER_H
#include <vector>
#include <memory>

// 缓冲区模型
// +----------------------+-----------------+-----------------------+
// |  pre_writeable_area  |  readable_area  |  post_writeable_area  |
// +----------------------+-----------------+-----------------------+
// ^                      ^                 ^                       ^
// |                      |                 |                       |
// 0                  read_index_      write_index_                 n
//
// 区域采用左闭右开区间的表示形式，即：
// 1. pre_writeable_area:       [0, read_index_)
// 2. readable_area:            [read_index_, write_index_)
// 3. post_writeable_area:      [write_index_, n)
// 
// 由此，可以退出3个区域的长度各自为
// 1. pre_writeable_area:       read_index_ - 0 = read_index
// 2. readable_area:            write_index_ - read_index_
// 3. post_writeable_area:      n - write_index_

// 字节缓冲区
class ByteBuffer
{
public:
    /// @brief 默认构造函数，初始化buffer_为指定大小的数组
    ByteBuffer(int capacity = 0);

    /// @brief 向缓冲区中写入内容 
    /// @param val 待写入的内容
    /// @param length 待写入内容的长度
    /// @return 实际写入的大小
    size_t write(const char* val, size_t length);

    /// @brief 向缓冲区中写入内容 
    /// @param val 待写入的内容
    /// @return 实际写入的大小 
    size_t write(const std::string& val);

    /// @brief 读取内容
    /// @param dst 读取内容写入的位置
    /// @param length 待读取的长度; 0 -- 读取全部内容，>0 读取指定长度内容
    /// @return 实际读取的长度
    size_t read(char* dst, size_t length);

    /// @brief 读取内容
    /// @param length 待读取的长度; 0 -- 读取全部内容，>0 读取指定长度内容
    /// @return 待读取内容转换的字符串
    std::string read(size_t length);

    /// @brief 释放readable_area的内容
    /// @param length 释放的长度
    /// @return 被释放的内容
    std::string release(size_t length);

    /// @brief 压缩缓冲区，前移readable_area中的内容，合并writeable_area
    void compact();

    /// @brief 获取缓冲区中可读内容的长度
    /// @return 可读内容长度
    size_t readable();

    /// @brief 获取缓冲区中可写区域的总长度
    /// @return 可写区域长度
    size_t writeable();

    /// @brief 输出缓冲区信息
    /// @return 缓冲区具体信息的字符串
    std::string to_string();

private:
    std::string buffer_;        // 字节数组，缓冲区
    size_t      read_index_;    // readable区域的起始位置指针 
    size_t      write_index_;   // writeable区域的起始位置指针
    size_t      capacity_;      // 缓冲区容量

    /// @brief 获取可读区域前的可写区域长度
    /// @return pre_writeable_area 的长度
    size_t pre_writeable();

    /// @brief 获取可读区域后的可写区域长度
    /// @return post_writeable_area 的长度
    size_t post_writeable();

    /// @brief 扩容buffer
    /// @param increment 需要增加的大小
    /// @return 扩容后的实际大小
    size_t extend(size_t increment);
};

#endif