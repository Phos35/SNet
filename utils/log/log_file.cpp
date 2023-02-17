#include "log_file.h"
#include "config.h"

// TODO 在配置文件中指定日志生成的目录
LogFile::LogFile()
:written_size_(0), written_times_(0),
 file_index_(0)
{
    // 读取参数
    Config &config = Config::get_instance();
    base_name_ = config.log_base_name();
    file_size_limit_ = config.log_file_size_limit();

    // 利用滚动创建日志文件
    roll_file();
}

void LogFile::write(const char *val)
{
    written_size_ += file_.write(val);
    ++written_times_;

    // 日志文件已达到最大，需要滚动
    if(written_size_ >= file_size_limit_)
    {
        roll_file();
    }

    /// 跨天滚动日志文件 
    if(written_times_ >= LOG_ROLL_CHECK_EVERY_N && 
       Timestamp::is_different_day(create_time_, Timestamp::now()) == false)
    {
        roll_file();
    }
}

void LogFile::roll_file()
{
    std::string file_name = get_new_name();
    FILE *file = fopen(file_name.c_str(), "a");
    assert(file != nullptr && "Log File Create Failed!");

    // 更换新的文件流
    file_.set_file(file);

    // 更新时间信息、写入次数、写入大小
    create_time_ = Timestamp::now();
    written_times_ = 0;
    written_size_ = 0;
}

std::string LogFile::get_new_name()
{
    // 同一天内的文件，以index做区分
    // 跨天重置index
    if(Timestamp::is_different_day(create_time_, Timestamp::now()) == true)
    {
        file_index_ = 0;
    }
    return Timestamp::now().format("%Y_%m_%d_") + base_name_ + "_" +
          std::to_string(file_index_++) + ".log";
}