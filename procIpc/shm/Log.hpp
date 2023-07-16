#ifndef _LOG_H_
#define _LOG_H_

#include <iostream>
#include <ctime>

#define NOTICE 0
#define WARNING 1
#define ERROR 2
#define DEBUG 3

const std::string msg[] = {
    "Notice",
    "Warning",
    "Error",
    "Debug"};
std::ostream &Log(std::string message, int level)
{
    std::cout << " | " << (unsigned)time(nullptr) << " | " << msg[level] << " | " << message;
    return std::cout;
}

#endif