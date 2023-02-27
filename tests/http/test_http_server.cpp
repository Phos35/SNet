#include "http_server.h"
#include "http_request.h"
#include "logger.h"
#include "http_dispatcher.h"
#include "config.h"
#include "http_response.h"
#include "interface.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <signal.h>

HTTPResponse on_slah(HTTPRequest* request)
{
    HTTPResponse response;
    std::ifstream file("index.html");
    if(file.is_open() == false)
    {
        LOG_ERROR << "File index.html open failed";
        response.set_code(404);
        response.set_description("Not Found");
        return response;
    }
    else
    {
        std::istreambuf_iterator<char> begin(file);
        std::istreambuf_iterator<char> end;
        std::string content(begin, end);

        response.set_content_type(HTTPResponse::MIME::HTML);
        response.add_content(content);
        file.close();   
        return response;
    }
}

HTTPResponse on_favicon(HTTPRequest* request)
{
    HTTPResponse response;

    std::ifstream file("favicon.ico");
    if (file.is_open() == false)
    {
        LOG_ERROR << "File favicon.ico open failed";
        response.set_code(404);
        response.set_description("Not Found");
        return response;
    }
    else
    {
        std::istreambuf_iterator<char> begin(file);
        std::istreambuf_iterator<char> end;
        std::string content(begin, end);

        response.set_content_type(HTTPResponse::MIME::IMAGE);
        response.add_content(content);
        file.close();
        return response;
    }
}

int main()
{
    signal(SIGPIPE, SIG_IGN);

    // 读取配置文件
    Config &config = Config::get_instance("/home/ubuntu/projects/web_server/SNet/config/config.json");

    // 启动异步日志
    Logger::enable_async();

    // 注册处理函数
    HTTPDispatcher::register_handler("/", on_slah);
    HTTPDispatcher::register_handler("/favicon.ico", on_favicon);

    // 创建HTTP服务器并运行
    HTTPServer server("0.0.0.0", 9737);
    server.run();

    return 0;
}