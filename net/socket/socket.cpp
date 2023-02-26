#include "socket.h"
#include "logger.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

/// @brief 创建一个socket
/// @param domain 协议族
/// @param type 数据类型
/// @param protocol 协议
Socket::Socket(int domain, int type, int protocol)
: domain_(domain), type_(type), protocol_(protocol)
{
    fd_ = socket(domain, type, protocol);
}

/// @brief 封装一个已经存在的fd
/// @param fd 代表socket的文件描述符
/// @param addr 地址
Socket::Socket(int fd, sockaddr_in *addr)
{
    fd_ = fd;
    domain_ = addr->sin_family;
    type_ = SOCK_STREAM;
    protocol_ = 0;
    memcpy(&addr_in_, addr, sizeof(addr_in_));
}

/// @brief 为socket绑定地址和端口
/// @param addr 指定地址
/// @param port 指定端口
/// @return 成功返回0，失败返回-1
int Socket::bind(const std::string &addr, short port)
{
    addr_in_.sin_family = domain_;
    addr_in_.sin_port = htons(port);
    int ret = inet_pton(domain_, addr.c_str(), &addr_in_.sin_addr.s_addr);
    if(ret == 0)
    {
        LOG_ERROR << "Invalid addr";
        return -1;
    }
    if(ret == -1)
    {
        LOG_ERROR << "Invalid AF family";
        return -1;
    }

    return ::bind(fd_, (sockaddr *)&addr_in_, sizeof(addr_in_));
}

/// @brief 为socket绑定地址和端口
/// @param addr 指定地址结构体
/// @return 成功返回0，失败返回-1
int Socket::bind(sockaddr_in *addr)
{
    memcpy(&addr_in_, addr, sizeof(addr_in_));
    return ::bind(fd_, (sockaddr *)addr, sizeof(addr_in_));
}

void Socket::shutdown(uint32_t flag)
{
    ::shutdown(fd_, flag);
}

int Socket::set_reuse_addr()
{
    int opt_val = 1;
    return setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, 
                      &opt_val, sizeof(opt_val));
}

int Socket::fd()
{
    return fd_;
}

/// @brief 以可读的形式获取socket绑定的地址
/// @return 地址字符串
std::string Socket::addr_str()
{
    char buf[32] = {0};
    inet_ntop(domain_, &addr_in_.sin_addr.s_addr, buf, 32);
    return buf;
}

/// @brief 获取socket绑定的端口号
/// @return 端口号
uint16_t Socket::port()
{
    return ntohs(addr_in_.sin_port);
}

Socket::~Socket()
{
    close(fd_);
}