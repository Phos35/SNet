#ifndef SNET_CONFIG_H
#define SNET_CONFIG_H
#include <string>
#include <json.hpp>
using json = nlohmann::json;

class Config
{
public:
    /// @brief 获取实例
    /// @return 唯一的Config实例
    static Config &get_instance();

    /// @brief 从指定的文件中获取配置信息
    /// @param file_name 配置文件的名称
    void read_from_file(const std::string &file_name);

    /// @brief 获取日志文件的基础名称
    /// @return 日志文件基础名称
    std::string log_base_name();

    /// @brief 获取单个日志文件大小限制
    /// @return 日志文件大小限制
    size_t log_file_size_limit();

    /// @brief 日志文件刷新缓冲区的时间间隔
    /// @return 秒级间隔
    size_t log_flush_interval();

    /// @brief 异步日志中后端线程被唤醒的时间间隔
    /// @return 秒级间隔
    size_t log_wakeup_interval();

private:
    // 禁止外部创建、拷贝
    Config();
    Config(const Config &);

    json configs_;      // json格式的配置参数
};

#endif