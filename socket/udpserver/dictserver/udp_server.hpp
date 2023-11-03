#ifndef _UDP_SERVRER_HPP
#define _UDP_SERVRER_HPP

#include <iostream>
#include <string>
#include <functional>
#include <cerrno>
#include <cstring>

#include <strings.h>

// 网络四件套头文件
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "log.hpp"

#define SIZE 1024

typedef std::function<void(int, std::string, uint16_t, std::string)> func_t;

class UdpServer
{
public:
    UdpServer(const uint16_t &port, func_t callback, const std::string &ip = "0.0.0.0") : _port(port), _ip(ip), _sockfd(-1), _callback(callback)
    {
    }

    void initServer()
    {
        // 1. 创建套接字
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd == -1)
        {
            logMessage(FATAL, "socket error, %d : %s", errno, strerror(errno));
            exit(1);
        }
        logMessage(NORMAL, "socket success, %d", _sockfd);

        // 2. 套接字和系统进行绑定
        struct sockaddr_in local;
        bzero(&local, sizeof local);
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = inet_addr(_ip.c_str());

        int n = bind(_sockfd, (sockaddr *)&local, sizeof(local));
        if (n == -1)
        {
            logMessage(FATAL, "bind error, %d : %s", errno, strerror(errno));
            exit(2);
        }
        logMessage(NORMAL, "bind success");

        // 预备工作完成
    }

    void start()
    {
        char buffer[SIZE];
        struct sockaddr_in peer;
        socklen_t len = sizeof peer;
        memset(&peer, 0, sizeof peer);
        while (true)
        {
            ssize_t s = recvfrom(_sockfd, &buffer, sizeof buffer, 0, (struct sockaddr *)&(peer), &len);
            if (s > 0)
            {
                buffer[s] = '\0';
                std::string clientip = inet_ntoa(peer.sin_addr);
                uint16_t clientport = ntohs(peer.sin_port);
                std::string message = buffer;
                std::cout << "client[" << clientip << "]:[" << clientport << "] wanto translate :" << message << std::endl;
                _callback(_sockfd, clientip, clientport, message);
            }
            else if (s == 0)
            {
                break;
            }
            else
            {
                // 读端关闭
                break;
            }

            // std::string returnMessage = "server says:" + std::string(buffer);
            // sendto(_sockfd, returnMessage.c_str(), returnMessage.size(), 0, (sockaddr *)&peer, len);
        }
    }
    ~UdpServer() {}

private:
    std::string _ip;
    uint16_t _port;
    int _sockfd;
    func_t _callback;
};

#endif