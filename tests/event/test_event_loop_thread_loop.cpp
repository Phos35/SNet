#include "event_loop_thread_pool.h"
#include "config.h"
#include "logger.h"
#include <iostream>

int main()
{
    Config &config = Config::get_instance("/home/ubuntu/projects/web_server/SNet/config/config.json");

    EventLoopThreadPool pool(4);
    pool.start();
    EventLoop *loop = pool.get_one();
    LOG_TRACE << "EventLoop " << loop->id() << " is selected";

    std::string input;
    while (input.find("QUIT") == std::string::npos)
    {
        std::cin >> input;
    }
    pool.quit_all();

    return 0;
}