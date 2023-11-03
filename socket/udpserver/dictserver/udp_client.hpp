#ifndef _UDP_CLIENT_HPP
#define _UDP_CLIENT_HPP

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>
#include <unistd.h>

#define SIZE 1024
class UdpClient
{
public:
    UdpClient(const std::string &serverip, const uint16_t &serverport) : _serverip(serverip), _serverport(serverport), _sockfd(-1)
    {
    }

    void initClient()
    {
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            std::cerr << "socket error" << std::endl;
            exit(1);
        }
        std::cout << "socket success" << std::endl;
        // 客户端不需要显示bind : OS随机分配端口
    }

    void run()
    {
        struct sockaddr_in server;
        memset(&server, 0, sizeof server);
        server.sin_addr.s_addr = inet_addr(_serverip.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(_serverport);
        socklen_t len = sizeof server;

        std::string message;
        char getmessage[SIZE];
        while (true)
        {
            std::cout << "请输入message:";

            std::cin >> message;

            sendto(_sockfd, message.c_str(), message.size(), 0, (struct sockaddr *)&server, len);
            ssize_t s = recvfrom(_sockfd, getmessage, sizeof getmessage, 0, nullptr, nullptr);
            getmessage[s] = 0;
            std::cout << "server sendback: " << std::string(getmessage) << std::endl;
        }
    }
    ~UdpClient()
    {
    }

private:
    int _sockfd;
    std::string _serverip;
    uint16_t _serverport;
};

#endif
