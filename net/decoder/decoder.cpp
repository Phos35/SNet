#include "decoder.h"

Decoder::Decoder()
{

}

Decoder::~Decoder()
{

}

Message::SPtr Decoder::decode(const std::string& data)
{
    // decoder基类直接返回一个原始数据Message
    Message::SPtr msg = std::make_shared<Message>();
    return msg;
}