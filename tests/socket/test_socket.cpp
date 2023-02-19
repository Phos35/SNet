#include "socket.h"
#include <memory>

void move_test(std::unique_ptr<Socket>&& tmp)
{
    std::unique_ptr<Socket> s(std::move(tmp));
    printf("The addr is: %s:%d\n", s->addr_str().c_str(), s->port());
}

int main()
{
    std::unique_ptr<Socket> soc = std::make_unique<Socket>(AF_INET, SOCK_STREAM, 0);
    soc->bind("127.0.0.1", 9119);
    soc->set_reuse_addr();
    move_test(std::move(soc));
    return 0;
}