#ifndef MY_HTTP_DEMO_H
#define MY_HTTP_DEMO_H
#include <string>
#include <unordered_map>
#include "http_server.h"
#include "config.h"
#include "logger.h"
#include "http_dispatcher.h"
#define CONFIG_FILE "../config/config.json"
#define ROOT "../resources"

class HTTPDemo
{
public:
    /// @brief 运行
    void run();

private:

    /// @brief 解析url请求资源的路径
    /// @param url 待解析的url
    /// @return 资源路径
    std::string parse_path(const std::string &url);

    /// @brief 获取指定路径的资源
    /// @param path 指定的路径
    /// @return 资源数据
    std::string get_resource(const std::string &path);

    /// @brief 若未找到资源，则返回404
    /// @return 404响应
    HTTPResponse handle_not_found();

    /// @brief 处理get请求
    /// @param request 待处理的请求
    /// @return 请求的数据响应或者404响应
    HTTPResponse handle_get(HTTPRequest* request);

    /// @brief 将十六进制字符串转换为对应的字符
    /// @param hexStr 十六进制字符串
    /// @return 对应字符
    char hexASCIItoChar(const std::string &hexStr);

    /// @brief 将数据中的特殊字符转换为正常字符
    /// @param ori 数据
    /// @return 正常字符串
    std::string cancelHTTPFormat(std::string &ori);

    /// @brief 解析POST请求中的请求参数
    /// @param args POST请求的content
    /// @return 请求参数map
    std::unordered_map<std::string, std::string> parse_args(std::string &args);

    /// @brief 处理post请求
    /// @param request 
    /// @return post请求的响应
    HTTPResponse handle_post(HTTPRequest *request);

    /// @brief 处理url = /上的请求
    /// @param requse 客户端请求
    /// @return 响应
    HTTPResponse handle_slash(HTTPRequest *request);
};

#endif