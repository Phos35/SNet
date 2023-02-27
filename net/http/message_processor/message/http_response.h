#ifndef SNET_HTTP_RESPONSE_H
#define SNET_HTTP_RESPONSE_H
#include <string>
#include <unordered_map>
#include "message.h"

/// TODOLIST
/// 1. 修正HTTPResponse、HTTPRequest的相关结构应当作为基类被继承的问题
/// 2. 修正MIME应当单独设置一个类并读取相关配置文件获取的问题
/// 3. 修正HTTPResponse头部字段获取需要上层用户转型的问题

class HTTPResponse : public Message
{
public:
    typedef std::unordered_map<std::string, std::string> HeaderMap;

    /// MIME类型
    enum class MIME
    {
        TEXT,   // 纯文本类型
        HTML,   // html文件
        IMAGE,  // 图片
        PNG,    // png图片
    };

    // HTTP响应
    HTTPResponse(const std::string &version = "HTTP/1.1", size_t code = 200,
                 const std::string &description = "OK");

    /// @brief 获取HTTP协议版本
    /// @return HTTP协议版本
    std::string version();

    /// @brief 设置HTTP协议版本
    /// @param version HTTP协议版本
    void set_version(const std::string &version);

    /// @brief 获取状态码
    /// @return 状态码
    size_t code();

    /// @brief 设置状态码
    /// @param code 待设置的值
    void set_code(size_t code);

    /// @brief 获取状态码描述
    /// @return 状态码描述
    std::string description();

    /// @brief 设置状态码描述
    /// @param val 新的描述字符串
    void set_description(const std::string &val);

    /// @brief 获取头部字段值
    /// @param key 头部字段的键
    /// @return 键对应的字符串值
    std::string get_header_val(const std::string &key);

    /// @brief 设置头部字段，若头部字段不存在则添加
    /// @param key 键
    /// @param val 值
    void set_header(const std::string &key, const std::string &val);

    /// @brief 设置Content-Type字段
    /// @param type 待设置的值
    void set_content_type(MIME type);

    /// @brief 添加响应体 -- 自动添加Content-Length字段
    /// @param content 待添加的内容
    void add_content(const std::string &content);

    /// @brief 添加响应体 -- 移动语义；自动添加Content-Length
    /// @param content 待添加的内容
    void add_content(std::string &&content);

    /// @brief 获取完整的响应报文
    /// @return 响应报文
    std::string data() override;

    /// @brief MIME转换为对应的字符串
    /// @param type 待转换的类型
    /// @return MIME type对应的字符串
    static std::string mime_to_string(MIME type);

private:
    std::string     version_;       // HTTP协议版本
    size_t          code_;          // 状态码
    std::string     description_;   // 状态码描述

    HeaderMap       headers_;       // 头部字段

    std::string     content_;       // 响应体

};

#endif