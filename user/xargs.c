#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
//该程序估计无法处理多行,只理解部分内容
int main(int argc,char *argv[])
{
    if(argc < 2) // 参数太少，报错退出
    {
        fprintf(2,"xargs to few argument ...\n");
        exit(1);
    }
    
    char *arr[MAXARG+1];
    int idx = 0;
    for(int i = 1; argv[i]; i++) // 读取命令+参数
    {
        arr[idx++] = argv[i];  
    }

    char buf[512];
    char *p = buf;//设计一个缓冲区
    while(read(0,p,1) == 1) // 1个字符1个字符的读取到buf中
    {
        if((*p) == '\n') // 当读取到\n，则命令+完整参数的一行读取完成
        {
            *p = 0; // 结尾至0
            int pid = fork();
            if(pid == 0)//开一个子进程执行程序
            {
                arr[idx] = buf;  // 不太清楚细节
                exec(argv[1],arr);
                fprintf(2, "exec %s failed\n", argv[1]);
                exit(0);
            }
            wait(0);
            p = buf;
        }
        else p++;
        
    }
    exit(0);
}