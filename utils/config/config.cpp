#include "config.h"
#include <fstream>
#include <stdlib.h>

bool Config::created_ = false;

Config& Config::get_instance(const std::string& filename)
{
    static Config config;
    if(created_ == false)
    {
        config.read_from_file(filename);
        created_ = true;
    }
    return config;
}

void Config::read_from_file(const std::string &file_name)
{
    std::ifstream input_file(file_name);
    assert(input_file.is_open() == true);

    configs_ = json::parse(input_file);
    input_file.close();
}

bool Config::log_async_start()
{
    assert(configs_["log"]["async_enable"].is_boolean() == true);
    return configs_["log"]["async_enable"];
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

size_t Config::log_flush_interval()
{
    assert(configs_["log"]["flush_interval"].is_number_unsigned() == true);
    return configs_["log"]["flush_interval"].get<size_t>();
}

size_t Config::log_wakeup_interval()
{
    assert(configs_["log"]["wakeup_interval"].is_number_unsigned() == true);
    return configs_["log"]["wakeup_interval"].get<size_t>();
}

std::string Config::event_iomutiplexing()
{
    assert(configs_["event"]["iomutiplexing"].is_string() == true);
    return configs_["event"]["iomutiplexing"];
}

size_t Config::event_loop_thread_pool_size()
{
    assert(configs_["tcp"]["event_loop_thread_cnt"].is_number_unsigned() == true);
    return configs_["tcp"]["event_loop_thread_cnt"].get<size_t>();
}

size_t Config::worker_thread_pool_size()
{
    assert(configs_["tcp"]["worker_thread_cnt"].is_number_unsigned() == true);
    return configs_["tcp"]["worker_thread_cnt"].get<size_t>();   
}

size_t Config::tasks_queue_capacity()
{
    assert(configs_["tcp"]["tasks_queue_size"].is_number_unsigned() == true);
    return configs_["tcp"]["tasks_queue_size"].get<size_t>();
}

Config::Config()
{

}

Config::Config(const Config &)
{

}