#pragma once

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <ctime>

// 日志是有日志级别的
#define DEBUG 0
#define NORMAL 1
#define WARNING 2
#define ERROR 3 // 有些错误 还能执行
#define FATAL 4 // 致命错误 不能执行

const char *gLevelMap[] = {
    "DEBUG",
    "NORMAL",
    "WARNING",
    "ERROR",
    "FATAL"};

// 完整的日志功能，至少: 日志等级 时间 支持用户自定义(日志内容, 文件行，文件名)
void logMessage(int level, const char *format, ...) // 类型为 const char *format 的若干个参数
{
    // va_list ap; // char *
    // va_start(ap, format);
    // int x = va_arg(ap, int);
    // va_end(ap);
    // 自己取参太麻烦

#ifndef DEBUG_SHOW
    if (level == DEBUG)
        return;
#endif
    char stdBuffer[1024];
    time_t timestamp = time(nullptr);
    // // struct tm *lt = localtime(&timestamp); // 通过时间戳timestamp去获得具体的年月日时分

    snprintf(stdBuffer, sizeof stdBuffer, "[%s][%ld]", gLevelMap[level], timestamp);
    char logBuffer[1024];
    va_list args;
    va_start(args, format);
    // vprintf(format, args);
    vsnprintf(logBuffer, sizeof logBuffer, format, args);
    // FILE *fp = fopen("log.txt", "a");
    // fprintf(fp, "%s%s\n", stdBuffer, logBuffer);
    // fclose(fp);
    printf("%s%s\n", stdBuffer, logBuffer);

    va_end(args);
}