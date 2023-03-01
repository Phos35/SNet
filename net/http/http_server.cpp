#include "http_server.h"
#include "http_message_processor_factory.h"

HTTPServer::HTTPServer(const std::string &addr, uint16_t port)
:TCPServer(addr, port)
{

}

void HTTPServer::create_factory()
{
    set_factory(std::make_unique<HTTPMessageProcessorFactory>());
}