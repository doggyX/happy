#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    pid_t id = fork();

    if (id == 0)
    {
        printf("我是子进程\n");
    }
    else
    {

        printf("我是父进程\n");
    }
}
