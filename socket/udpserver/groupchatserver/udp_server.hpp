#ifndef _UDP_SERVRER_HPP
#define _UDP_SERVRER_HPP

#include <iostream>
#include <unordered_map>
#include <queue>
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
    UdpServer(const uint16_t &port, const std::string &ip = "0.0.0.0") : _port(port), _ip(ip), _sockfd(-1)
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
        char key[64];
        struct sockaddr_in peer;
        socklen_t len = sizeof peer;
        std::cout << "start" << std::endl;
        while (true)
        {
            memset(&peer, 0, sizeof peer);
            ssize_t s = recvfrom(_sockfd, &buffer, sizeof buffer, 0, (struct sockaddr *)&(peer), &len);
            std::cout << "catch" << std::endl;

            std::string clientip = inet_ntoa(peer.sin_addr);
            uint16_t clientport = ntohs(peer.sin_port);
            snprintf(key, sizeof key, "%s-%d", clientip.c_str(), clientport);
            logMessage(NORMAL, "key: %s 用户发来了消息 ", key);
            if (_users.find(key) == _users.end())
            {
                _users.insert({key, peer});
                logMessage(NORMAL, "添加了新用户：%s ", key);
            }

            if (s > 0)
            {
                buffer[s] = '\0';

                std::string message = key;
                message += buffer;

                for (auto user : _users)
                {
                    sendto(_sockfd, message.c_str(), message.size(), 0, (sockaddr *)&peer, len);
                }
            }
        }
    }
    ~UdpServer() {}

private:
    std::string _ip;
    uint16_t _port;
    int _sockfd;
    std::unordered_map<std::string, struct sockaddr_in> _users;
    // std::queue<std::string> _messageQueue;
};

#endif