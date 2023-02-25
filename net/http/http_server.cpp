#include "http_server.h"
#include "http_decoder.h"
#include "http_dispatcher.h"

HTTPServer::HTTPServer(const std::string &addr, uint16_t port)
:TCPServer(addr, port)
{
    //TODO 注册默认情况的处理函数

}

void HTTPServer::create_decoder()
{
    set_decoder(new HTTPDecoder());
}

void HTTPServer::create_dispatcher()
{
    set_dispatcher(new HTTPDispatcher());
}