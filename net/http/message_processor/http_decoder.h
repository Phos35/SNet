#ifndef SNET_HTTP_DECODER_H
#define SNET_HTTP_DECODER_H
#include "decoder.h"
#include "http_request.h"

// 报文解析错误码
using DecodeError = HTTPRequest::Error;

class HTTPDecoder : public Decoder
{
public:
    // 报文解析状态
    enum class DecodeState
    {
        REQUEST_LINE,   // 解析请求行
        HEADERS,        // 解析头部字段
        CONTENT,        // 解析内容
        FINISH,         // 解析完成
    };

    /// @brief 解析HTTP报文
    /// @param data 待解析的数据
    /// @return 解析完成的报文
    Message* decode(const std::string &data) override;
private:
    size_t              cur_index_;     // 报文解析中的当前指针
    size_t              line_start_;    // 当前行的起始位置
    size_t              line_end_;      // 当前行的结束位置

    DecodeState         state_;         // 解析状态
    HTTPRequest*        request_;       // 请求报文指针，用于解析过程中临时存储

    /// @brief 解析请求行
    /// @param data 待解析的数据
    /// @return 解析错误码
    DecodeError decode_request_line(const std::string &data);

    /// @brief 解析请求方法
    /// @param data 待解析的数据
    /// @return 解析错误码
    DecodeError decode_method(const std::string &data);

    /// @brief 解析url
    /// @param data 待解析的数据
    /// @return 解析错误码
    DecodeError decode_url(const std::string &data);

    /// @brief 预处理url，+、16进制值等转化实际字符
    /// @param data 待处理的数据
    /// @param url 存储处理完成的url
    /// @return 解析错误码
    DecodeError preprocess_url(const std::string &data, std::string& url);

    /// @brief 解析url中的16进制字符
    /// @param start %所在位置的索引
    /// @param data 待解析的数据
    /// @param url 已完成解析的url
    /// @return 错误码
    DecodeError decode_hex(int& start, const std::string &data, std::string& url);

    /// @brief 解析resource-target中的query部分
    /// @param query_data 待解析的query部分数据
    /// @return 解析错误码
    DecodeError decode_query(const std::string &query_data);

    /// @brief 解析HTTP协议版本
    /// @param data 待解析的数据
    /// @return 解析错误码
    DecodeError decode_version(const std::string &data);

    /// @brief 解析头部字段
    /// @param data 待解析的数据
    /// @return 解析错误码
    DecodeError decode_headers(const std::string &data);

    /// @brief 解析报文内容部分
    /// @param data 待解析的数据
    /// @return 解析错误码
    DecodeError decode_content(const std::string &data);

    /// @brief 读取一行的
    /// @param data 
    /// @return 
    DecodeError get_one_line(const std::string &data);
};

#endif