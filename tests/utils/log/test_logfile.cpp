#include "logger.h"
#include "log_file.h"
#include "config.h"
#include <iostream>

int main()
{
    Config& config = Config::get_instance();
    config.read_from_file("/home/ubuntu/projects/web_server/SNet/config/config.json");
    LogFile lf;
    Logger::set_output_callback(std::bind(&LogFile::write, &lf, std::placeholders::_1));

    std::string input;
    int written = 0;
    while (true)
    {
        std::cin >> input;
        LOG_DEBUG << input;

        written += input.size();
        printf("written: %d\n\n", written);
    }
    return 0;
}