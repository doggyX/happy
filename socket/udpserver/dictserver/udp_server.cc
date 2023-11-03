/*
2023/9/13 socket编程
bzero(&local, sizeof(local)); // 把字段全清零
*/

#include "udp_server.hpp"
#include <memory> // 智能指针
#include <stdlib.h>
#include <fstream>
#include <unordered_map>

const std::string dictTxt = "./dict.txt";
std::unordered_map<std::string, std::string> dict;

static bool cutString(const std::string &line, std::string &key, std::string &value, const std::string &sep)
{
    auto pos = line.find(sep);
    if (pos == std::string::npos)
        return false;
    key = line.substr(0, pos);
    value = line.substr(pos + sep.size());
    return true;
}

static void initDict()
{
    std::ifstream in(dictTxt, std::ios::binary);
    if (!in.is_open())
    {
        std::cerr << "open file error :" << errno << strerror(errno) << std::endl;
        exit(1);
    }
    std::string line;
    std::string key, value;

    while (getline(in, line))
    {
        if (cutString(line, key, value, ":"))
        {
            dict.insert({key, value});
        }
    }
    in.close();
    logMessage(NORMAL, "load dict success");
}

static void usage(std::string proc)
{
    std::cout << "\nUsage: " << proc << " ip port\n"
              << std::endl;
}

void handlerMessage(int sockfd, std::string clientip, uint16_t clientport, std::string message)
{
    std::string response_message;
    auto iter = dict.find(message);
    if (iter == dict.end())
    {
        response_message = "unknown";
    }
    else
    {
        response_message = iter->second;
    }

    sockaddr_in client;
    bzero(&client, sizeof client);
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr(clientip.c_str());
    client.sin_port = htons(clientport);
    sendto(sockfd, response_message.c_str(), response_message.size(), 0, (sockaddr *)&client, sizeof client);
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
    std::unique_ptr<UdpServer> svr(new UdpServer(port, handlerMessage));

    initDict();
    // debug
    // for (auto kv : dict)
    // {
    //     std::cout << kv.first << ":" << kv.second << std::endl;
    // }
    svr->initServer();
    svr->start();
}