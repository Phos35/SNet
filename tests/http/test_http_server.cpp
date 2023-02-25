#include "http_server.h"
#include "http_request.h"
#include "logger.h"
#include "http_dispatcher.h"
#include "config.h"
#include "http_response.h"
#include <fstream>

void on_slah(const HTTPConnSPtr& conn, HTTPRequest* request)
{
    std::ifstream file("index.html");
    if(file.is_open() == false)
    {
        LOG_ERROR << "File index.html open failed";
    }
    else
    {
        char buf[1024] = {0};
        file.readsome(buf, 1024);

        HTTPResponse response;
        response.set_content_type(HTTPResponse::MIME::TEXT);
        response.add_content(buf);

        conn->write(response.data());
    }
    conn->close();
}

int main()
{
    // 读取配置文件
    Config &config = Config::get_instance("/home/ubuntu/projects/web_server/SNet/config/config.json");

    // 注册处理函数
    HTTPDispatcher::register_handler("/", on_slah);

    // 创建HTTP服务器并运行
    HTTPServer server("0.0.0.0", 9737);
    server.run();

    return 0;
}