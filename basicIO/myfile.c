#include <stdio.h>
#include <unistd.h> 
#include <string.h>
int main(int argc, char* argv[])
{
  if(argc != 2)
    return 1;
  
  FILE* fp = fopen(argv[1],"r");
  if(!fp)
  {
    perror("fopen");
    return 2;
  }
  char line[64];
  //按行读
  while(fgets(line,sizeof(line),fp))
  {
    fprintf(stdout,"%s",line);
  }
// const char *s1 = "hello fwrite\n"; 
// const char *s2 = "hello fprintf\n"; 
// const char *s3 = "hello fputs\n"; 
//  fwrite(s1,strlen(s1),1,fp);
//  fprintf(fp,"%s",s2);
//  fputs(s3,fp);
  fclose(fp);
}
