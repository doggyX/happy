/*
2023/9/13 socket编程
bzero(&local, sizeof(local)); // 把字段全清零
*/

#include "udp_server.hpp"
#include <memory> // 智能指针
#include <stdlib.h>

static void usage(std::string proc)
{
    std::cout << "\nUsage: " << proc << " ip port\n"
              << std::endl;
}

// ./udp_server ip port //云服务器的问题？ bug
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        usage(argv[0]);
        exit(1);
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    int sock = 1;
    std::unique_ptr<UdpServer> svr(new UdpServer(port, ip));

    svr->initServer();
    svr->Start();
}