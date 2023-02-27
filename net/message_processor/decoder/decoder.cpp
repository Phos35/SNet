#include "decoder.h"

Decoder::Decoder()
{

}

Decoder::~Decoder()
{

}

Message* Decoder::decode(const std::string& data)
{
    return new Message(data);
}