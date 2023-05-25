#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
  pid_t pid=fork();
  if(pid == 0)
  {
     printf("i am test\n");
      //execl("/usr/bin/ls","ls","-a","-l",NULL);
      exit(1);
  }
  else
  {
      //int status=0;
      //int res = waitpid(-1,&status,0);//res就是儿子的pid
      //printf("res:%d\n",res);
  }
}
