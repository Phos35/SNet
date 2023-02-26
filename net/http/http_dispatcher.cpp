#include "http_dispatcher.h"
#include "logger.h"
#include "tcp_connection.h"
#include "message.h"

HTTPDispatcher::HandlerTable HTTPDispatcher::handler_table_;

void HTTPDispatcher::dispatch(const TCPConnPtr &conn, const Message::SPtr& msg)
{
    HTTPRequest::SPtr http_request = std::dynamic_pointer_cast<HTTPRequest>(msg);

    // 若request的状态有问题，则根据相应问题响应
    if(http_request->error() != HTTPRequest::Error::NO_ERROR)
    {
        std::string error_str = HTTPRequest::error_to_string(http_request->error());
        handler_table_[error_str](conn, http_request);
        return;
    }

    // 若函数表无对应的url处理函数，则输出错误消息，关闭连接
    HTTPRequest::URL url = http_request->get_url();
    auto itr = handler_table_.find(url);
    if (itr == handler_table_.end())
    {
        LOG_ERROR << "URL: " << url << ", has no handler to handle";
        conn->close();
    }
    // 否则根据url选择函数进行处理
    else
    {
        itr->second(conn, http_request);
    }
}

void HTTPDispatcher::register_handler(const std::string& url, const Handler& handler)
{
    handler_table_[url] = handler;
}