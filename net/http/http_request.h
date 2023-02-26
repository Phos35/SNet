#ifndef SNET_HTTP_REQUEST_H
#define SNET_HTTP_REQUEST_H
#include "message.h"
#include <string>
#include <unordered_map>

class HTTPRequest : public Message
{
public:
    typedef HTTPRequest *Ptr;
    typedef std::shared_ptr<HTTPRequest> SPtr;
    typedef std::string URL;
    typedef std::string Version;
    typedef std::unordered_map<std::string, std::string> HeaderMap;
    typedef std::unordered_map<std::string, std::string> QueryMap;
    typedef std::string Content;

    // 请求方法
    enum Method
    {
        UNKNOWN,    // 未知方法
        GET,        // 请求指定的资源，并返回资源实体
        HEAD,       // 请求指定资源的信息，即GET请求中的headers部分
        POST,       // 客户端提交数据请求处理，可能会获取或改动服务端资源
        PUT,        // 用客户端传输的数据替代指定的资源
        DELETE,     // 请求删除指定资源
        CONNECT,    // TODO 待理解
        OPTIONS,    // TODO 待理解
        TRACE,      // echo，用于测试
        PATCH,      // PUT的补充，用于对已有资源进行局部更新
    };

    /// 报文的解析错误信息
    enum class Error
    {
        NO_ERROR,       // 报文解析正常，无错误

        UNKNOWN_ERROR,  // 未知错误

        BAD_LINE,       // 一行未以rn换行符结尾
        UNCOMPLETE,     // 报文不全

        BAD_METHOD,     // 错误的请求方法，如不存在或者拼写错误
        BAD_URL,        // URL语法错误
        BAD_VERSION,    // 错误的HTTP版本，如1.2等

        BAD_HEADER,     // header语法错误，如:右侧没有空格
    };

    HTTPRequest(const std::string& data);

    /// @brief 获取错误码
    /// @return 错误码
    Error error();

    /// @brief 设置错误码
    /// @param error 待设置的错误码
    void set_error(Error error);

    /// @brief 获取请求方法
    /// @return 请求方法
    Method get_method();

    /// @brief 设置请求方法
    /// @param method 待设置的请求方法
    void set_method(Method method);

    /// @brief 获取请求方法对应的字符串
    /// @return 请求方法对应的字符串
    std::string get_method_str();

    /// @brief 设置请求方法
    /// @param method 请求方法对应的字符串
    void set_method(std::string method);

    /// @brief 获取url
    /// @return url
    URL get_url();

    /// @brief 设置url
    /// @param url 待设置的url
    void set_url(const std::string &url);

    /// @brief 获取协议版本
    /// @return 协议版本
    Version get_version();

    /// @brief 设置协议版本
    /// @param version 
    void set_version(Version version);

    /// @brief 获取字段值
    /// @param key 字段值对应的键
    /// @return 字符串类型的字段值
    std::string get_header_val(const std::string &key);

    /// @brief 添加头部字段
    /// @param key 头部字段的键
    /// @param val 头部字段的值
    void add_header(const std::string &key, const std::string &val);

    /// @brief 获取content的引用
    /// @return content的引用
    Content &content_ref();
    
    /// @brief 设置content
    /// @param content 待设置的content
    void set_content(const std::string& content);

    /// @brief 获取queries的引用
    /// @return queries引用
    QueryMap &queries_ref();

    /// @brief 报文相关信息字符串
    std::string to_string();

    /// @brief 将请求方法转换为对应的字符串
    /// @param method 请求方法
    /// @return 请求方法对应的字符串
    static std::string method_to_string(Method method);

    /// @brief 将请求方法字符串转化为对应的请求方法
    /// @param method_str 请求方法字符串
    /// @return 字符串对应的请求方法
    static Method string_to_method(const std::string &method_str);

    /// @brief 将错误码转换为对应的字符串
    /// @param error 待转换的字符串
    /// @return 错误码对应的字符串
    static std::string error_to_string(Error error);

    /// @brief 检验请求方法是否有效
    /// @param method 待检验的请求方法
    /// @return 有效返回true，无效返回false
    static bool method_valid(const std::string& method);

    /// @brief 检验版本是否有效
    /// @param version 待检验的版本
    /// @return 有效则返回true，无效则返回false
    static bool version_valid(const std::string &version);

private:
    Method      method_;    // 请求方法
    URL         url_;       // 请求url
    Version     version_;   // HTTP版本

    HeaderMap   headers_;   // 头部字段
    Content     content_;   // 内容

    QueryMap    queries_;   // query参数

    Error       error_;     // 错误信息

    // 字符串与请求方法的对应表
    static std::unordered_map<std::string, Method> string_method_table;
};

#endif