#include "acceptor.h"
#include "config.h"

void new_conn(std::unique_ptr<Socket>&& tmp)
{
    std::unique_ptr<Socket> s(std::move(tmp));
    printf("New Connect: %s:%hu\n", s->addr_str().c_str(), s->port());
    while(true)
        ;
}

int main()
{
    Config &config = Config::get_instance("/home/ubuntu/projects/web_server/SNet/config/config.json");

    Socket soc(AF_INET, SOCK_STREAM, 0);
    soc.set_reuse_addr();
    soc.bind("127.0.0.1", 9112);

    Acceptor a(soc.fd());
    a.set_conn_callback(new_conn);
    a.start();
    while(true)
        ;
    return 0;
}