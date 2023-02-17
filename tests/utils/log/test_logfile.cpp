#include "logger.h"
#include "log_file.h"
#include "config.h"

int main()
{
    Config& config = Config::get_instance();
    config.read_from_file("/home/ubuntu/projects/web_server/SNet/config/config.json");
    LogFile lf;
    Logger::set_output_callback(std::bind(&LogFile::write, &lf, std::placeholders::_1));

    LOG_TRACE << "This is a trace msg";
    return 0;
}