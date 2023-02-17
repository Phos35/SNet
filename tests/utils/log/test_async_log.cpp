#include "async_log.h"
#include <iostream>
#include <string>
#include "config.h"
#include "logger.h"

int main()
{
    Config &config = Config::get_instance();
    config.read_from_file("/home/ubuntu/projects/web_server/SNet/config/config.json");

    Logger::enable_async();
    std::string input;
    while (input.find("QUIT") == std::string::npos)
    {
        std::cin >> input;
        LOG_DEBUG << input;
    }

    return 0;
}