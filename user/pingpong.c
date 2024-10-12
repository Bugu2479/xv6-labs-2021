#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"  


int main(int argc,char * argv[])
{   
    int ptoc[2]; 
    int ctop[2];
    pipe(ptoc);// 开2个管道，一个父向子通信，一个反的
    pipe(ctop);
    char a = 0;
    char *buff = &a; // read,write 需要用 字符指针,‘‘是字符，“”才是字符串
    int pid = fork(); // 父进程得到子进程id，不是自己的id
    if(pid > 0)  // 父进程执行这个
    {
        write(ptoc[1],"b",1); //父进程 向管道写数据，写完关闭
        close(ptoc[1]);    
        wait(0);  // 等到子进程退出，再从管道中读数据
        if(read(ctop[0],buff,1)  )
        {
            fprintf(1,"%d: received pong\n",getpid());
            close(ctop[0]);
            exit(0);
        }
        else 
        {
            close(ctop[0]);
            exit(1);
        }   
    }
    if(pid == 0) // 子进程执行这个
    {
        
        pid = getpid(); //获取子进程pid
        if(read(ptoc[0],buff,1)) // 从管道中读数据 
        {
            fprintf(1,"%d: received ping\n",pid);
            close(ptoc[0]);
        }  
        else exit(1);
        
        if (write(ctop[1],buff,1) )
        {
            close(ctop[1]);
            exit(0);
        }
        else 
        {
            close(ctop[1]);
            exit(1);
        }
    }
    else
    {
        fprintf(2,"fork error!");
    }


    
}