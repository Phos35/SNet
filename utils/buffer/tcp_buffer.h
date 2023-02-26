#ifndef SNET_TCP_BUFFER_H
#define SNET_TCP_BUFFER_H
#include "byte_buffer.h"
/*TCPBuffer竞争*/
// 1. 读取fd时可能对端只发送了一部分上层需要的数据，如HTTP报文中只发送了RequestLine而剩余部分上位来得及到达。
//    这种情况下，一份报文就可能被交个Decoder中不同的工作线程处理，导致了报文解析的失败，因此，需要考虑一种策略，
//    使得每份HTTP报文只被一个工作线程处理。
// 
// 2. 方法一：使用bool变量守护Buffer，暂且称之为守护变量
//    这种方法工作如下：
//    每次写入Buffer后，将守护变量置为false，直到Decoder中解析报文完成再将守护变量置为true，
//    而当守护变量为false时不允许再次写入，同时TCPConnection也不会将buffer递交给Decoder。
//    从而保证了一份报文只能由一个工作线程进行解析。
//    缺陷：在LT模式下，若我们不及时读取fd中的数据，则readable时间会被不断触发，进而导致
//    TCPConnection所属的事件循环不断被唤醒却不能做任何工作，也就是busy-loop，这样的开销太大
//
// 3. 方法二：Decoder工作线程拷贝数据，待解析完成后通知TCPConnection释放Buuffer
//    这种方法工作如下：
//    TCPConnection将数据交给Decoder，Decoder选择工作线程后等待工作线程将数据拷贝完成，随后
//    工作线程开始解析报文，且TCPConnection所在时间循环执行其他任务（此时TCPConnection中的Buffer数据不做修改）。
//    等待工作线程解析完成后，通知TCPConnection的事件循环，进而在TCPConnection中释放对应的数据内容
//    优点：避免的方法一中的busy-loop
//    缺点：数据拷贝的过程会延迟从Decoder返回TCPconnection的过程；数据拷贝造成了冗余；通知事件循环需要依赖于Fucntor
//    这可能会在事件循环忙碌的时候造成更大的负担（这一点存疑，这种情况下应该修正事件循环中处理Functor的策略）
// 
// 4. 方法一、二需要实践中衡量两者差距，目前选择方法二进行实现

class TCPBuffer : public ByteBuffer
{
public:
    TCPBuffer(int capacity = 0);

    /// @brief 从fd对应的socket上读取数据，写入到buffer中
    /// @param fd 待读取的fd
    /// @return 发生错误返回-1，否则返回实际读取到的大小
    int read_from_fd(int fd);

private:
    /// @brief buffer写满，扩容后写入
    /// @param data 待写入的数据
    /// @param size 数据长度
    void extend_write(char *data, int size);

    size_t lower_half_cnt_; // 记录写入buffer_的数据小于buffer_一半的次数，用于定期缩小buffer_
};

#endif