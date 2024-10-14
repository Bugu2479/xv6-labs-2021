#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"  
#define MAXNUM 36
#define R 0
#define W 1
//设计思路：要实现对0~35的数字筛选出素数（只能被自己和1整除），我们设计一个字符数组
//保存素数是否被筛除，然后fork一个子进程，把字符数组通过管道传输
//子进程设计为递归，从左管道中读取字符数组，筛去能被当前最小数的整除的数，然后把修改好的字符数组传入右管道
//当子进程完成自己的任务后，fork一个孙进程，让孙进程继续筛选，直到全部筛选完成返回
void  primes(int left,int right)
{
    int first  = 0 ;
    char nums[MAXNUM];
    read(left,nums,MAXNUM); // 从管道中读取数据
    for(int i = 0; i < MAXNUM; i++)
    {
        if(nums[i] == '0')
        {
            first = i;
            break;
        } 
    }
    if(first == 0) // 所有数据处理完了，first 不会变化，退出程序
    {
        exit(0);
    }
    fprintf(1,"prime %d\n",first); // 打印当前判断出来的素数
    for(int i = 0; i < MAXNUM; i++)
    {
        if(i % first == 0) // 能被整除，则不是素数，标记1
        {
            nums[i] = '1';
        }
    
    }
    int pid = fork();
    if(pid > 0)
    {
        write(right,nums,MAXNUM); 
        wait(0);
    }
    else if(pid == 0)
    {
        primes(left,right);
        wait(0);
    }
}

int main(int argc,char * argv[])
{
    int p[2];
    char nums[MAXNUM] ;
     memset(nums,'0',sizeof(nums)); // 初始化数组 ，重要！
    pipe(p);
    int pid = fork();
    if(pid > 0)
    {
        nums[0] = '1';// '1' 表示该下标代表的数不是素数或者已经被当成素数输出了，'0'表示还没有判断出该数不是素数
        nums[1] = '1'; 
        write(p[W],nums,MAXNUM); // 传入数据到管道
        wait(0);
    }
    else if(pid == 0)
    {
        primes(p[R],p[W]);
        exit(0);
    }
    else
    {
        fprintf(2,"fork error!\n");
        exit(1);
    }
    exit(0);

}