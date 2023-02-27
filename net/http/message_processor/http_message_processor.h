#ifndef SNET_HTTP_MESSAGE_PROCESSOR_H
#define SNET_HTTP_MESSAGE_PROCESSOR_H
#include "message_processor.h"
#include "http_response.h"

class HTTPMessageProcessor : public MessageProcessor
{
public:
    /// @brief 创建Decoder和Dispatcher
    void create()override;

protected :
    /// @brief 单个响应发送完毕后对连接的处理 -- 关闭或保持
    /// @param conn TCP连接
    /// @param request 客户端请求
    void process_conn(const TCPConnSPtr &conn, Message *request) override;

private:

};

#endif