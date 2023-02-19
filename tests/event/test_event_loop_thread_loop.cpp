#include "event_loop_thread_pool.h"
#include "config.h"
#include "logger.h"

int main()
{
    Config &config = Config::get_instance("/home/ubuntu/projects/web_server/SNet/config/config.json");

    EventLoopThreadPool pool(4);
    pool.start();
    EventLoop *loop = pool.get_one();
    LOG_TRACE << "EventLoop " << loop->id() << " is selected";

    return 0;
}