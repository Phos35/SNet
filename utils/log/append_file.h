#ifndef SNET_APPEND_FILE
#define SNET_APEPND_FILE
#include <string>
#include <fstream>
#define FLUSH_FREQUENCY 1024

class AppendFile
{
public:
    AppendFile(FILE* file = nullptr);
    ~AppendFile();

    /// @brief 向文件中写出数据
    /// @param val 待写出的数据
    /// @return 实际写出的长度
    size_t write(const char *val);

    /// @brief 设置输出的文件流
    /// @param file 文件流
    void set_file(FILE *file);

    // 刷新文件缓冲区
    void flush();

private:    
    size_t  write_cnt_; // 写出次数，用于定期刷新缓冲区

    FILE*   file_;      // 写出的目标文件
};

#endif