#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* // 函数 fmtname 对一个字符串末尾保存的文件名进行修改，确保返回的文件名长度符合固定标准，若不足则用空格填充。
fmtname(char *path) 
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--) // ?
    ;
  p++;// 此时 p 指向文件名部分的起始位置  

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
   memmove(buf, p, strlen(p)); // 确保文件名符合规范
   memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}
void find(char *path, char *object) // find函数，第1变量为路径，第2个为查找的文件名
{
    
    char buf[512],*p;
    int fd;
    struct dirent de; // 目录文件中目录条的信息
    struct stat st; // 每个文件的状态信息
    if((fd = open(path,0)) < 0) // 确保文件可以打开
    {
        fprintf(2,"find can not open %s\n",path);
        return ;
    }
    if((fstat(fd,&st)) < 0)   // 确保文件状态信息可以获取
    {
        fprintf(2,"find can not stat %s ",path);
        close(fd);
        return ;
    } 
    if(st.type != T_DIR) //对第一次 path 路径检查是否为目录文件
    {
        fprintf(2,"path is not directory!\n");
        return ;
    }
    
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){ // 给出目录文件变量太长了
      fprintf(2,"ls: path too long\n");
      return ;
    }

    strcpy(buf, path); //复制路径到缓冲区
    p = buf+strlen(buf); //指针指向缓冲区末尾
    *p++ = '/'; // p指针指向末尾/后的第一个字符
    while(read(fd, &de, sizeof(de)) == sizeof(de)){  //读取目录文件，每次读de格式大小的内容，当读取大小不对则读取完成
      if(de.inum == 0) // inum为0，表示文件无效
        continue;
      memmove(p, de.name, DIRSIZ); //将读取到的文件或目录名保存到buf末尾中
      p[DIRSIZ] = 0; //缓冲区添加/0，分隔开

      if(stat(buf, &st) < 0)
      { // 获取新的文件信息
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
        //对读取到的文件进行判断是目录还是文件，是文件判断是否是想找的，是目录则递归进去查找
        if(st.type == T_DIR && strcmp(de.name,".") != 0 && strcmp(de.name,"..") != 0)
        {
            find(buf,object);
        }
        else
        {
            if(strcmp(p,object) == 0)
            {
                printf("%s\n",buf);
            }
        }
    }
  close(fd);

}
int main(int argc, char *argv[])  
{
    if(argc <= 1) // 没有变量的情况
    {
       printf("find not have argument :( \n");
       exit(0);
    }
    if(argc == 2) // 只给了一个变量，在当前目录中查找
    {
        find(".",argv[1]);  
        exit(0);
    }
    if(argc > 3) // 给多了变量，这里错误直接输出。!! 后续可以升级该功能，多线程查找2个
    {
        printf("to much more argument !! \n");
        exit(0);
    }
    find(argv[1],argv[2]); // 正确执行查找
    exit(0);
}