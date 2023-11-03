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
#include <pthread.h>

#include "threadPool/thread.hpp"
#define SIZE 1024

struct ServerData
{
    int _sockfd;
    std::string _serverip;
    uint16_t _serverport;

    ServerData(int sockfd, std::string serverip, uint16_t serverport) : _sockfd(sockfd), _serverip(serverip), _serverport(serverport)
    {
    }
};

class UdpClient
{
public:
    UdpClient(const std::string &serverip, const uint16_t &serverport) : _serverip(serverip), _serverport(serverport), _sockfd(-1)
    {
    }

    static void *sendfunc(void *args)
    {
        pthread_detach(pthread_self());
        ThreadData *td = (ThreadData *)args; // 死亡 多綫程卡在這裏
        std::cout << "run--- sockfd:" << ((ServerData *)(td->args_))->_sockfd << std::endl;
        ServerData *sd = (ServerData *)td->args_;
        std::cout << "send fd" << sd->_sockfd << std::endl;

        struct sockaddr_in server;
        memset(&server, 0, sizeof server);
        server.sin_addr.s_addr = inet_addr(sd->_serverip.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(sd->_serverport);
        socklen_t len = sizeof server;

        std::string message;
        while (true)
        {
            std::cout << "请输入message:";

            std::cin >> message;

            int n = sendto(sd->_sockfd, message.c_str(), message.size(), 0, (struct sockaddr *)&server, len);
            std::cout << "n : " << std::string(strerror(errno)) << "sock" << sd->_sockfd << std::endl;
        }
    }

    static void *recivfunc(void *args)
    {
        pthread_detach(pthread_self());
        ThreadData *td = (ThreadData *)args;
        ServerData *sd = (ServerData *)td->args_;
        struct sockaddr_in server;
        memset(&server, 0, sizeof server);
        server.sin_addr.s_addr = inet_addr(sd->_serverip.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(sd->_serverport);
        socklen_t len = sizeof server;
        char getmessage[SIZE];
        while (true)
        {
            ssize_t s = recvfrom(sd->_sockfd, getmessage, sizeof getmessage, 0, nullptr, nullptr);
            if (s > 0)
            {
                getmessage[s] = 0;
                std::cout << "server sendback: " << std::string(getmessage) << std::endl;
            }
        }
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
        _serverdata = new ServerData(_sockfd, _serverip, _serverport);

        ThreadData *td = new ThreadData();
        td->args_ = (void *)_serverdata;
        Thread *sender = new Thread(1, sendfunc, (void *)td);
        Thread *reciver = new Thread(2, recivfunc, (void *)td);

        sender->start();
        reciver->start();
        while (true)
            sleep(1);
    }
    ~UdpClient()
    {
        delete _serverdata;
    }

private:
    int _sockfd;
    std::string _serverip;
    uint16_t _serverport;
    ServerData *_serverdata;
};

#endif
