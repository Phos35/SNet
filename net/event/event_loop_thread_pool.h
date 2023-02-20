#ifndef SNET_EVENT_LOOP_THREAD_POOL
#define SNET_EVENT_LOOP_THREAD_POOL
#include "event_loop_thread.h"
#include <vector>

// TODO commit a41676147f63f8e542637e9247e150b06a38644c
//      下EventLoopThreadPool无法正常退出线程
//   而 commit 4bdb72197977e7940f8625eb7d994e0c68997ec6
//      在EventLoop、EventLoopThread中添加了状态成员、
//      修改EventLoopThreadPool中quit_all为pop_back()后
//      正常运行，原因尚不明确，需要查明

class EventLoopThreadPool
{
public:
    /// @brief 创建指定大小的线程池
    /// @param size 指定的大小
    EventLoopThreadPool(size_t size);

    ~EventLoopThreadPool();

    /// @brief 启动线程池
    void start();

    /// @brief 结束所有事件循环线程
    void quit_all();

    /// @brief 获取一个事件循环
    /// @return 被选中的事件循环指针
    EventLoop* get_one();

private:
    typedef EventLoopThread ELThread;
    typedef std::unique_ptr<ELThread> ELThreadPtr;
    std::vector<ELThreadPtr>        threads_;          // 线程列表
    size_t                          threads_cnt_;   // 线程数量
    bool                            running_;       // 线程池仍在运行的标志

    /// @brief 负载均衡机制，默认RR
    /// @return 被选中的事件循环指针
    EventLoop *balance_select();
};

#endif
