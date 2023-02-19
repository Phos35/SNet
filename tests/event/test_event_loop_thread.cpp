#include "event_loop_thread.h"
#include "config.h"
#include <string>
#include <iostream>

int main()
{
    Config &config = Config::get_instance("/home/ubuntu/projects/web_server/SNet/config/config.json");

    EventLoopThread evt;

    evt.start();
    std::string input;
    while (input.find("QUIT") == std::string::npos)
    {
        std::cin >> input;
        printf("input: %s", input.c_str());
    }
    evt.quit();

    return 0;
}