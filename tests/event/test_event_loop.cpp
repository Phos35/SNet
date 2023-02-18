#include "event_loop.h"
#include <thread>
#include "config.h"
#include <unistd.h>

EventLoop *loop;

// 跨线程启动断言测试
void test_assert()
{
    loop->start();
}

/// 事件循环线程函数
void loop_thread_fun()
{
    loop = new EventLoop();
    loop->start();
}

/// 跨线程调用函数
void functor()
{
    printf("This is a functor\n");
}

// 事件读处理函数
void process_read()
{
    char buf[64] = {0};
    read(0, buf, 64);
    printf("This is a read processor, read: %s\n", buf);
}

int main()
{
    Config &config = Config::get_instance("/home/ubuntu/projects/web_server/SNet/config/config.json");

    // 测试同线程断言
    // loop = new EventLoop();
    // std::thread th(test_assert);
    // th.join();

    // 测试跨线程调用
    // std::thread th(loop_thread_fun);
    // th.detach();
    // sleep(10);
    // loop->run_in_loop(functor);

    // 测试添加事件
    std::thread th(loop_thread_fun);
    sleep(1);
    Event e(0, EPOLLIN);
    e.set_read_callback(process_read);
    loop->add_event(e);

    th.join();

    return 0;
}