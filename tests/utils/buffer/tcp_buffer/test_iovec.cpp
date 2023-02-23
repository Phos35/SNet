#include <string>
#include <vector>
#include <sys/uio.h>
#include <iostream>
#include <unistd.h>
#include "tcp_buffer.h"
#include "socket.h"
#include "acceptor.h"
#include "config.h"

using SocketPtr = std::unique_ptr<Socket>;
void new_conn(SocketPtr &&client)
{
    SocketPtr local = std::move(client);
    TCPBuffer tb(8);
    while (true)
    {
        int read_size = tb.read_from_fd(local->fd());
        if(read_size == 0)
        {
            sleep(1);
            continue;
        }
        std::string str = tb.read(0);

        if(str.find("END") != std::string::npos)
            break;
        else if(str.find("RELEASE") != std::string::npos)
            tb.release(5);
        else if(str.find("READ") != std::string::npos)
            std::cout << tb.read(5);
        else if(str.find("COMPACT") != std::string::npos)
            {}// bb.compact(); 测试在该函数是public时进行，权限修改后取消


        std::cout << tb.to_string();
    }
}

int main()
{
    Config &config = Config::get_instance("/home/ubuntu/projects/web_server/SNet/config/config.json");

    Socket sock(AF_INET, SOCK_STREAM, 0);
    sock.set_reuse_addr();
    sock.bind("127.0.0.1", 9112);
    Acceptor a(sock.fd());
    a.set_conn_callback(new_conn);
    a.start();
    while(true)
        ;
    return 0;
}