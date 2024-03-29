#ifndef SNET_LOGGER_H
#define SNET_LOGGER_H

#include <thread>
#include <pthread.h>
#include "log_stream.h"
#include "timestamp.h"
#include "callback.h"
#include "async_log.h"

class Logger
{
public:
    // 日志等级
    enum class LogLevel
    {
        FATAL = 0,
        ERROR,
        WARN,
        INFO,
        DEBUG,
        TRACE
    };

    Logger(LogLevel level, const std::string &file, const std::string& func, int line);
    ~Logger();

    /// @brief 启用异步日志
    static void enable_async();

    /// @brief 设置输出回调函数
    /// @param cb 回调函数
    static void set_output_callback(const OutputCallBack& cb);

    /// @brief 获取输出流
    /// @return 日志输出流
    LogStream &stream();

    /// @brief 设置日志输出等级
    /// @param level 目标等级
    static void set_log_level(LogLevel level);

    /// @brief 获取日志输出等级
    /// @return 日志输出等级
    static LogLevel get_log_level();

private:    
    LogStream               stream_;        // 日志输出流
    static AsyncLog         async_;         // 异步日志类
    static bool             async_enableed_;// 异步日志启用的标志

    Timestamp               timestamp_;     // Logger生成的时间戳
    pid_t                   tid_;           // 线程id
    static LogLevel         level_;         // 日志输出等级
    static std::string      levels_str[];   // 日志输出等级对应的字符串

    std::string             file_;          // 打印日志的源文件名称
    std::string             func_;          // 打印日志的函数名
    int                     line_;          // 打印日志的行号

    static OutputCallBack   output_fun_;    // 输出回调函数

    /// @brief 默认的输出回调函数。输出到stdcout
    /// @param val 待输出的数据
    static void default_output(const char* val);

    /// @brief 调用输出回调函数写出日志
    void output();

};

// 定义宏，便于使用
#define LOG(level)  if (level <= Logger::get_log_level()) Logger(level, __FILE__, __func__, __LINE__).stream()
#define LOG_TRACE LOG(Logger::LogLevel::TRACE)
#define LOG_DEBUG LOG(Logger::LogLevel::DEBUG)
#define LOG_INFO LOG(Logger::LogLevel::INFO)
#define LOG_WARN LOG(Logger::LogLevel::WARN)
#define LOG_ERROR LOG(Logger::LogLevel::ERROR)
#define LOG_FATAL LOG(Logger::LogLevel::FATAL)

#endif