#ifndef _UDP_SERVRER_HPP
#define _UDP_SERVRER_HPP

#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>

#include <strings.h>

// 网络四件套头文件
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "log.hpp"

#define SIZE 1024

class UdpServer
{
public:
    UdpServer(uint16_t port, std::string ip = "0.0.0.0") : _port(port), _ip(ip), _sock(-1)
    {
    }

    bool initServer()
    {
        // 从这里开始就是新的系统调用，来完成网络功能了
        // 1. 创建套接字
        // socket函数, 文件描述符被返回
        _sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sock < 0)
        {
            logMessage(FATAL, "%d:%s", errno, strerror(errno)); // 看一下被设置的错误码
            exit(2);
        }

        // 2. bind绑定 - 将用户设置的ip和port在内核中和当前进程强关联 成功返回0
        // "192.168.1.3" -> 点分十进制字符串风格的IP地址 ->是给用户看的
        // 每一个区域取值范围是[0-255]: 1字节 -> 4个区域
        // 理论上, 表示一个IP地址, 四字节就够了 -> 网络实际用的
        // 点分十进制字符串风格的IP地址 <-> 4字节
        struct sockaddr_in local;
        bzero(&local, sizeof(local)); // 把字段全清零
        local.sin_family = AF_INET;

        // 服务器的IP和端口未来也是要发给对方主机的 -> 先要将数据发送到网络!
        local.sin_port = htons(_port); // 主机序列转网络序列

        // 1. 同上, 先要将点分十进制字符串风格的IP地址转换为4字节
        // 2. 4字节主机序列 -> 网络序列
        // 有一套接口, 可以一次帮我们做完这两件事
        local.sin_addr.s_addr = inet_addr(_ip.c_str());

        if (bind(_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            logMessage(FATAL, "%d:%s", errno, strerror(errno)); // 看一下被设置的错误码
            exit(2);
        }
        logMessage(NORMAL, "init udp server done ... %s", strerror(errno));
        return true;
    }

    void Start()
    {
        // 作为一款网络服务器, 是永远不退出的!
        // 服务器启动 -> 进程 -> 常驻进程 -> 永远在内存中存在,除非挂了! -- 内存泄漏问题就很重要了
        char buffer[SIZE];
        for (;;)
        {
            // 注意:
            // peer,纯输出性参数
            struct sockaddr_in peer;
            bzero(&peer, sizeof(peer));
            // 输入: peer 缓冲区的大小
            // 输出: 实际读取到的peer
            socklen_t len = sizeof(peer);
            // start. 读取数据
            ssize_t s = recvfrom(_sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&peer, &len);
            if (s > 0)
            {
                buffer[s] = 0; // 我们目前的数据当做字符串
                // 1. 输出发送的数据信息
                // 2. 是谁
                uint16_t cli_port = ntohs(peer.sin_port);      // 从网络中来的
                std::string cli_ip = inet_ntoa(peer.sin_addr); // 4字节的网络序列的IP->本主机的字符串风格的IP
            }
            // 分析和处理数据
            // end. 写回数据
        }
    }
    ~UdpServer()
    {
    }

private:
    // 一个服务器必需要ip地址和port(16位整数)

    uint16_t _port;
    std::string _ip;
    int _sock;
};

#endif