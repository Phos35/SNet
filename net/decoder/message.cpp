#include "message.h"

Message::Message()
:result_(DeocdeResult::FAILURE)
{

}

Message::Message(const std::string& data)
:result_(DeocdeResult::SUCCESS), raw_data_(data)
{

}

Message::Message(DeocdeResult result)
: result_(result)
{

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

std::string Message::raw_data()
{
    return raw_data_;
}

size_t Message::raw_size()
{
    return raw_data_.size();
}