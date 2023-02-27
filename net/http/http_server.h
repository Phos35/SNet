#ifndef SNET_HTTP_SERVER_H
#define SNET_HTTP_SERVER_H
#include "tcp_server.h"

typedef TCPConnection HTTPConnection;
typedef TCPConnection::SPtr HTTPConnSPtr;

class HTTPServer : public TCPServer
{
public:
    /// @brief HTTP服务器
    /// @param addr 绑定的地址
    /// @param port 绑定的端口
    HTTPServer(const std::string &addr, uint16_t port);

protected:
    /// @brief 创建消息处理器工厂
    void create_factory() override;

private:

};

#endif