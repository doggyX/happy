/*
2023/9/13 socket编程
bzero(&local, sizeof(local)); // 把字段全清零
*/

#include "udp_server.hpp"
#include <memory> // 智能指针
#include <stdlib.h>
#include <fstream>
#include <unordered_map>

static void usage(std::string proc)
{
    std::cout << "\nUsage: " << proc << " ip port\n"
              << std::endl;
}

// ./udp_server  port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        usage(argv[0]);
        exit(1);
    }
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<UdpServer> svr(new UdpServer(port));
    svr->initServer();
    svr->start();
}