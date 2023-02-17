#include "logger.h"

Logger::LogLevel Logger::level_ = Logger::LogLevel::TRACE;
std::string Logger::levels_str[] = {"FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE"};

Logger::Logger(LogLevel level, const std::string &file, const std::string& func, int line)
:timestamp_(Timestamp::now()), tid_(pthread_self()), 
 file_(file), func_(func), line_(line), 
 output_fun_(std::bind(&Logger::default_output, this, std::placeholders::_1))
{
    stream_ << timestamp_.format("%Y-%m-%d %H:%M:%S.%s") << "  "
            << tid_ << "  " << levels_str[static_cast<int>(level)] 
            << "  ";
}

Logger::~Logger()
{
    stream_ << "  " << file_ << "  " << func_ << "  " << line_ << "\n";
    output();
}

/// TODO 添加AsyncLog的前端输入函数
void Logger::enable_async()
{

}

void Logger::set_output_callback(const OutputCallBack& cb)
{
    output_fun_ = std::move(cb);
}

void Logger::default_output(const char* val)
{
    printf("%s\n", val);
}

void Logger::output()
{
    output_fun_(stream_.data());
}

LogStream& Logger::stream()
{
    return stream_;
}

void Logger::set_log_level(LogLevel level)
{
    level_ = level;
}

Logger::LogLevel Logger::get_log_level()
{
    return level_;
}