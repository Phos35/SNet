#include "decoder.h"

Decoder::Decoder()
{

}

Decoder::~Decoder()
{

}

Message* Decoder::decode(const std::string& data)
{
    // decoder基类直接返回一个原始数据Message
    Message* msg = new Message(data);
    return msg;
}