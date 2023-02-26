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

/*注意！*/
// 此处使用string作为缓冲区是存在问题的，理由如下：
// 1. 当TCPBuffer使用data()->char*写入时，实际调用的时string.data()，而这个函数的返回值是const的，
//    因此这种做法从语言层面上来讲是一种bad behavior；
//    同时，在事件操作方面，直接写string.data()返回的指针将不会更新string内部的size等属性，也就意
//    味着string作为一个容器已经失效了。关于这一点，实际上可以容忍，因为数据的大小全部由ByteBuffer
//    中的三个指针接管，而实际上不需要string自身的size。使用string只是为了扩容机制。
//    但是考虑到第一条——将const指针转化为非const，有可能会因为udefined behavior而在某些平台上报错，
//    因此最好还是改为使用vector<char> 

// 字节缓冲区
class ByteBuffer
{
public:
    /// @brief 默认构造函数，初始化buffer_为指定大小的数组
    ByteBuffer(int capacity = 0);

    /// @brief 获取post_writeable_area的指针
    /// @return post_writeable_are的指针
    char *writeable_area();

    /// @brief 获取可读区域--readable_area的指针
    /// @return 可读区域指针
    char *readable_area();

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
    void release(size_t length);

    /// @brief 获取缓冲区中可读内容的长度
    /// @return 可读内容长度
    size_t readable();

    /// @brief 获取缓冲区中可写区域的总长度
    /// @return 可写区域长度
    size_t writeable();

    /// @brief 获取缓冲区容量大小
    /// @return 容量大小
    size_t capacity();

    /// @brief 输出缓冲区信息
    /// @return 缓冲区具体信息的字符串
    virtual std::string to_string();

protected:
    /// @brief 获取最低容量
    /// @return 最低容量
    size_t base_capacity();

    /// @brief 压缩缓冲区，前移readable_area中的内容，合并writeable_area
    void compact();

    /// @brief 扩容buffer
    /// @param increment 需要增加的大小
    /// @return 扩容后的实际大小
    size_t extend(size_t increment);

    /// @brief 缩小buffer
    /// @param decrement 需要缩小的大小
    /// @return 缩小后的实际大小
    size_t shrink(size_t decrement);

    /// @brief 增加write_index_
    /// @param increment 增量
    void increase_write_index(size_t increment);

private:
    std::vector<char>   buffer_;        // 字节数组，缓冲区
    size_t              read_index_;    // readable区域的起始位置指针 
    size_t              write_index_;   // writeable区域的起始位置指针
    size_t              capacity_;      // 缓冲区容量
    size_t              base_capacity_; // 最低容量大小


    /// @brief 获取可读区域前的可写区域长度
    /// @return pre_writeable_area 的长度
    size_t pre_writeable();

    /// @brief 获取可读区域后的可写区域长度
    /// @return post_writeable_area 的长度
    size_t post_writeable();
};

#endif