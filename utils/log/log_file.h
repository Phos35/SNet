#ifndef SNET_LOG_FILE_H
#define SNET_LOG_FILE_H
#include "append_file.h"
#include "timestamp.h"

#define LOG_ROLL_CHECK_EVERY_N 1024

class LogFile
{
public:
    // 读取日志相关的配置参数，创建新的日志文件
    LogFile();

    /// @brief 向文件中写出数据
    /// @param val 待写出的数据
    void write(const char *val);

    /// 滚动日志文件
    void roll_file();

    /// @brief 获取新的日志文件的名称
    /// @return 日志文件名称
    std::string get_new_name();

private:
    AppendFile  file_;              // 用于写出文件

    size_t      written_times_;     // 写入次数，用于定期滚动日志文件
    size_t      written_size_;      // 写入数据大小，用于定期滚动日志文件
    Timestamp   create_time_;       // 日志文件创建时间，用于跨天时滚动日志文件

    std::string base_name_;         // 日志文件的基础名称
    size_t      file_size_limit_;   // 日志文件大小限制
};

#endif