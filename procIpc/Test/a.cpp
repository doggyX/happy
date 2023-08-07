#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main()
{
    umask(0);
    int fd = open("./haha.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
    char buffer[64] = "hehe a\n";
    while (true)
    {
        sleep(2);
        write(fd, buffer, strlen(buffer));
    }
}
