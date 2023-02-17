#include "async_log.h"
#include <iostream>
#include <string>
#include "config.h"

int main()
{
    Config &config = Config::get_instance();
    config.read_from_file("/home/ubuntu/projects/web_server/SNet/config/config.json");

    AsyncLog alog;
    std::string input;
    while (input.find("QUIT") == std::string::npos)
    {
        std::cin >> input;
        alog.write(input.c_str());
    }

    return 0;
}