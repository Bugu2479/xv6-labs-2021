#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h" //该文档保存了各个系统调用的声明，当该程序如果用到其他系统调用时，能够运行

int main(int argc,char * argv[])
{
    unsigned  num ;
    if(argc != 2 )
    {
        write(1,"sleep error!",12);
        exit(1);
    }
    num = atoi(argv[1]);
    sleep(num);
    exit(0);
}