#include "http_message_processor.h"
#include "http_decoder.h"
#include "http_dispatcher.h"
#include "tcp_connection.h"

void HTTPMessageProcessor::create()
{
    set_decoder(new HTTPDecoder());
    set_dispatcher(new HTTPDispatcher());
}

void HTTPMessageProcessor::process_conn(const TCPConnSPtr &conn, Message *request)
{
    HTTPRequest* http_request = dynamic_cast<HTTPRequest *>(request);

    // 若存在Connection: Keep-Alive选项则存活，否则关闭连接
    std::string keep = http_request->get_header_val("Connection");
    if(keep != "Keep-Alive")
        conn->close();
}