#include "config.h"
#include <fstream>
#include <stdlib.h>

Config& Config::get_instance()
{
    static Config config;
    return config;
}

/// @brief 从指定的文件中获取配置信息
/// @param file_name 配置文件的名称
void Config::read_from_file(const std::string &file_name)
{
    std::ifstream input_file(file_name);
    assert(input_file.is_open() == true);

    configs_ = json::parse(input_file);
    input_file.close();
}

/// @brief 获取日志文件的基础名称
/// @return 日志文件基础名称
std::string Config::log_base_name()
{
    return configs_["log"]["base_name"];
}

Config::Config()
{

}

Config::Config(const Config &)
{

}