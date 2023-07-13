#ifndef _COMM_H_
#define _COMM_H_

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include "Log.hpp"

using namespace std;

string ipcPath = "./fifo.ipc";

#define MODE 0666
#define SIZE 128

#endif