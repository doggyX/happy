#pragma once

#include <iostream>
#include <assert.h>
#include <cstdio>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include "Log.hpp"

using namespace std;

#define PATH_NAME "/home/acs" // 最好要保证自己有访问权限
#define PROJ_ID 0x66
#define SHM_SIZE 4096 // 共享内存的大小，最好是页的整数倍（4096即(4KB)）