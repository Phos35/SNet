#include "message.h"

Message::Message()
:result_(DeocdeResult::FAILURE)
{

}

Message::Message(const std::string& data)
:result_(DeocdeResult::SUCCESS), data_(data)
{

}

Message::Message(DeocdeResult result)
: result_(result)
{

}

Message& Message::operator=(Message &&another)
{
    data_.swap(another.data_);
    return *this;
}

Message::~Message()
{

}

void Message::set_result(DeocdeResult result)
{
    result_ = result;
}

Message::DeocdeResult Message::get_result()
{
    return result_;
}

void Message::set_data(const std::string &data)
{
    data_ = data;
}

std::string Message::data()
{
    // 基类直接返回原始数据
    return data_;

    // 派生类需要将报文转换为字符串返回
}

size_t Message::data_size()
{
    // 基类返回字符串大小
    return data_.size();

    // 派生类需要根据实际情况返回值
    // 如HTTPResponse只需要在最终获取数据前生成data
    // 在此之前应当如何获取数据大小？
    // 保持一个size_成员，每次增加内容时增加该值，
    // 这样就算报文还未构造成字符串也能获取对应的大小
}

void Message::clear_data()
{
    data_.clear();
}