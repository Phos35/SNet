#ifndef SNET_DECODER_H
#define SNET_DECODER_H
#include "message.h"
#include <string>

class Decoder
{
public:
    typedef Decoder *Ptr;
    typedef std::unique_ptr<Decoder> UPtr;

    Decoder();
    virtual ~Decoder();

    /// @brief 解析数据
    /// @param data 待解析的数据
    /// @return 解析完成的报文的指针
    virtual Message::SPtr decode(const std::string& data);

private:
};

#endif