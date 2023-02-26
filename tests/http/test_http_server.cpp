#include "http_server.h"
#include "http_request.h"
#include "logger.h"
#include "http_dispatcher.h"
#include "config.h"
#include "http_response.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <signal.h>

void on_slah(const HTTPConnSPtr& conn, const HTTPRequest::SPtr& request)
{
    std::ifstream file("index.html");
    if(file.is_open() == false)
    {
        LOG_ERROR << "File index.html open failed";
    }
    else
    {
        std::istreambuf_iterator<char> begin(file);
        std::istreambuf_iterator<char> end;
        std::string content(begin, end);

        HTTPResponse response;
        response.set_content_type(HTTPResponse::MIME::HTML);
        response.add_content(content);

        conn->write(response.data());
    }
    file.close();
    conn->close();
}

void on_favicon(const HTTPConnSPtr& conn, const HTTPRequest::SPtr& request)
{
    std::ifstream file("favicon.ico");
    if (file.is_open() == false)
    {
        LOG_ERROR << "File favicon.ico open failed";
    }
    else
    {
        std::istreambuf_iterator<char> begin(file);
        std::istreambuf_iterator<char> end;
        std::string content(begin, end);

        HTTPResponse response;
        response.set_content_type(HTTPResponse::MIME::IMAGE);
        response.add_content(content);

        conn->write(response.data());
    }
    file.close();
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