#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc,char *argv[])
{
    if(argc < 0) // 无参数处理 
    {
        fprintf(2,"unabled to sleep ...");
        exit(1);
    }
    
    int i = 0;
    i = atoi(argv[1]);
     sleep(i);
     fprintf(1,"sleep good !");

    exit(0);
}