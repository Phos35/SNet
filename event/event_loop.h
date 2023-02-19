#ifndef SNET_EVENT_LOOP_H
#define SNET_EVENT_LOOP_H
#include "iomutiplexing.h"
#include "event.h"
#include "callback.h"
#include <pthread.h>
#include <memory>
#include <mutex>
#include <sys/types.h>
/*注意点：
1. wake_fd_用于唤醒EventLoop，其使用场景有如下两种
    1.1 EventLoop退出。如果只是单纯将running_置为false，loop()是可能不会退出的，因为loop()每次循环都可能
        阻塞在IOMutiplexing上，而如果长时间没有其关注的事件发生，那么就无法结束循环。
        使用wake_fd_可以在quit()函数中通过写wake_fd_而将loop()从IOMutiplexing的阻塞中唤醒，进而使其看到
        running_ == false而退出
    
    1.2 及时处理跨线程调用。跨线程调用的处理在loop()循环中位于IOMutiplexing之后，同上面所述，对于跨线程
        调用的处理也可能因为阻塞在IOMutiplexing中而无法及时处理。使用wake_fd_可以在每次添加开线程调用时
        唤醒loop()，从而避免因关注事件长时间不发生而导致跨线程调用没有得到及时处理的情况
*/


class EventLoop
{
public:
    EventLoop();
    ~EventLoop();

    /// @brief 启动循环
    void start();

    /// @brief 退出事件循环
    void quit();

    /// @brief 添加事件
    /// @param e 待添加的事件
    void add_event(Event &e);

    /// @brief 添加跨线程调用
    /// @param functor 跨线程调用函数
    void run_in_loop(const Functor &functor);

    /// @brief 获取EventLoop所在线程的id
    /// @return 线程id
    pid_t id();

private:
    typedef std::unique_ptr<IOMutiplexing> IOMutiplexingPtr;
    pid_t                   id_;            // EventLoop所在线程的线程id

    IOMutiplexingPtr        poller_;        // 监听事件的IOMutiplexing机制
    std::vector<Event>      fired_;         // 活跃事件列表

    std::mutex              functor_mtx_;   // functors_的访问互斥锁
    std::vector<Functor>    functors_;      // 跨线程调用列表
    int                     wake_fd_;       // 唤醒eventloop的eventfd

    bool                    running_;       // 事件循环运行标识

    /// @brief 循环函数
    void loop();

    /// @brief 判断是否在事件循环所属的线程中
    /// @return 若在则返回true，否则返回false
    bool in_loop_thread();

    /// @brief 断言在事件循环所属线程中
    void assert_in_loop_thread();

    /// @brief 在事件循环内添加事件
    /// @param e 待添加的事件
    void add_event_in_loop(Event &e);

    /// @brief 处理活跃事件
    /// @param fired_cnt 活跃事件的数量
    void process_fired(size_t fired_cnt);

    /// @brief 处理跨线程调用
    void process_functor();

    /// @brief 唤醒事件循环
    void wakeup();

    /// @brief 读取wake_fd_，避免wake_fd_重复触发读事件
    void read_wake_fd();

    /// @brief 添加跨线程调用
    /// @param fucntor 跨线程调用
    void add_functor(const Functor &functor);
};

#endif