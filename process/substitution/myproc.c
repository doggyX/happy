#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

#define NUM 16
char* a[NUM]={"ls","-a","-l",NULL};
int main()
{
  printf("----begin----\n");
  a[0]=(char*)"./main.py";
  //execl("./test","test",NULL);
  //execv("./main.py",a);
  execvp("ls",a);
 printf("----end------\n");
}
