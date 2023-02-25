#include "http_server.h"
#include "http_request.h"
#include "logger.h"
#include "http_dispatcher.h"
#include "config.h"

void on_slah(const HTTPConnSPtr& conn, HTTPRequest* request)
{
    printf("Request:\n%s\n---------------------\n", request->to_string().c_str());
    conn->write("OK!");
    conn->close();
}

int main()
{
    // 读取配置文件
    Config &config = Config::get_instance("/home/ubuntu/projects/web_server/SNet/config/config.json");

    // 注册处理函数
    HTTPDispatcher::register_handler("/", on_slah);

    // 创建HTTP服务器并运行
    HTTPServer server("0.0.0.0", 9379);
    server.run();

    return 0;
}