#ifndef SNET_SOCKET_H
#define SNET_SOCKET_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <memory>

// socket RAII封装
class Socket
{
public:
    typedef std::unique_ptr<Socket> UPtr;

    /// @brief 创建一个socket
    /// @param domain 协议族
    /// @param type 数据类型
    /// @param protocol 协议
    Socket(int domain, int type, int protocol);

    /// @brief 封装一个已经存在的fd
    /// @param fd 代表socket的文件描述符
    /// @param addr 地址
    Socket(int fd, sockaddr_in *addr);

    /// @brief 为socket绑定地址和端口
    /// @param addr 指定地址
    /// @param port 指定端口
    /// @return 成功返回0，失败返回-1
    int bind(const std::string &addr, short port);

    /// @brief 为socket绑定地址和端口
    /// @param addr 指定地址结构体
    /// @return 成功返回0，失败返回-1
    int bind(sockaddr_in *addr);

    /// @brief 获取socket的文件描述符
    /// @return 文件描述符
    int fd();

    /// @brief 设置端口复用
    /// @return 成功返回0，失败返回-1
    int set_reuse_addr();

    /// @brief 以可读的形式获取socket绑定的地址
    /// @return 地址字符串
    std::string addr_str();

    /// @brief 获取socket绑定的端口号
    /// @return 端口号
    short port();

    ~Socket();

private:
    int         fd_;        // 代表socket的文件描述符
    sockaddr_in addr_in_;   // 地址结构体，存储地址和端口

    int         domain_;    // 协议族
    int         type_;      // 通讯数据类型
    int         protocol_;  // 协议
};

#endif