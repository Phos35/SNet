#include "config.h"

int main()
{
    std::string filename = "/home/ubuntu/projects/web_server/SNet/config/config.json";

    Config &config = Config::get_instance();
    config.read_from_file(filename);
    printf("basename: %s\n", config.log_base_name().c_str());
    return 0;
}