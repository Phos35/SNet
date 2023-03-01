#include "http_dispatcher.h"
#include "logger.h"
#include "tcp_connection.h"
#include "message.h"
#include "debug.h"

HTTPDispatcher::HandlerTable HTTPDispatcher::handler_table_;

HTTPDispatcher::HTTPDispatcher()
{
    response_ = new HTTPResponse();
}

HTTPDispatcher::~HTTPDispatcher()
{
    delete response_;
}

Message* HTTPDispatcher::dispatch(Message* msg)
{
    HTTPRequest* http_request = dynamic_cast<HTTPRequest*>(msg);

    // 若request的状态有问题，则根据相应问题响应
    if(http_request->error() != HTTPRequest::Error::NO_ERROR)
    {
        return nullptr;
        // std::string error_str = HTTPRequest::error_to_string(http_request->error());
        // return handler_table_[error_str](http_request);
    }

    // 若函数表无对应的url处理函数，则输出错误消息，返回nullptr关闭连接
    HTTPRequest::URL url = http_request->get_url();
    auto itr = handler_table_.find(url);
    if (itr == handler_table_.end())
    {
        LOG_ERROR << "URL: " << url << ", has no handler to handle";
    }
    // 否则根据url选择函数进行处理
    else
    {
        response_->reset();
        *response_ = std::move(itr->second(http_request));
        LOG_DEBUG << "Dispatcher response data: " << SNet::DEBUG::one_line(response_->data());
        return response_;
    }
    return nullptr;
}

void HTTPDispatcher::register_handler(const std::string& url, const Handler& handler)
{
    handler_table_[url] = handler;
}