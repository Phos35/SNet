#include "append_file.h"
#include <cstring>

AppendFile::AppendFile(FILE* file)
:file_(file)
{

}

AppendFile::~AppendFile()
{
    if(file_ != nullptr)
    {
        fclose(file_);
        file_ = nullptr;
    }
}

size_t AppendFile::write(const char *val)
{
    int written = 0;
    int written_once = fwrite(val, 1, strlen(val), file_);
    written += written_once;
    while (written < strlen(val))
    {
        written_once = fwrite(val, 1, strlen(val) - written, file_);
        written += written_once;
    }

    write_cnt_++;
    if(write_cnt_ == FLUSH_FREQUENCY)
    {
        flush();
    }
    return written;
}

void AppendFile::set_file(FILE *file)
{
    // 若旧文件未关闭，则关闭后再获取新值
    if(file_ != nullptr)
    {
        fclose(file_);
        file_ = nullptr;
    }

    file_ = file;
    write_cnt_ = 0;
}

void AppendFile::flush()
{
    fflush(file_);
    write_cnt_ = 0;
}