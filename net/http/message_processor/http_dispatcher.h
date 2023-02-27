#ifndef SNET_HTTP_DISPATCHER
#define SNET_HTTP_DISPATCHER
#include "dispatcher.h"
#include "http_request.h"
#include <unordered_map>
#include <functional>

class HTTPDispatcher : public Dispatcher
{
public:
    typedef std::function<Message*(Message*)> Handler;
    typedef std::unordered_map<std::string, Handler> HandlerTable;

    /// @brief 将消息分配到对应的处理函数
    /// @param conn 消息发生的连接
    /// @param msg 待处理的消息
    Message* dispatch(Message* msg) override;

    /// @brief 注册处理函数
    /// @param 注册函数需要处理的url
    /// @param  待注册的函数
    static void register_handler(const std::string& url, const Handler& handler);

    /// @brief HTTP消息处理函数，形式为<url, handler_function>
    static HandlerTable handler_table_;

private:

};

#endif