#include "config.h"
#include <fstream>
#include <stdlib.h>

Config& Config::get_instance()
{
    static Config config;
    return config;
}

void Config::read_from_file(const std::string &file_name)
{
    std::ifstream input_file(file_name);
    assert(input_file.is_open() == true);

    configs_ = json::parse(input_file);
    input_file.close();
}

std::string Config::log_base_name()
{
    assert(configs_["log"]["base_name"].is_string() == true);
    return configs_["log"]["base_name"];
}

size_t Config::log_file_size_limit()
{
    assert(configs_["log"]["file_size_limit"].is_number_unsigned() == true);
    return configs_["log"]["file_size_limit"].get<size_t>();
}

Config::Config()
{

}

Config::Config(const Config &)
{

}