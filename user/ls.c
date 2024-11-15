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

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de; // 创建一个目录文件中文件的格式
  struct stat st;   // 创建一个文件信息， 用于后续获取文件信息后保存

  if((fd = open(path, 0)) < 0){ // 确保文件正确打开
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){  // 获取文件信息到 st
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }


  switch(st.type){  // 根据文件类型不同 做出不同处理
  case T_FILE:
    printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size); // 打印文件信息
    break;

  case T_DIR: 
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){ // 给出目录文件变量太长了
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path); //使用 strcpy 和 strcat（通过指针操作实现）构建读取目录条目的完整路径。
    p = buf+strlen(buf); //指针指向buf末尾
    *p++ = '/'; // 给buff末尾添加一个/元素后，指向末尾（还没填元素）
    while(read(fd, &de, sizeof(de)) == sizeof(de)){  //fd 指向目录文件，每次读de格式大小的内容，当读取大小不对则读取完成
      if(de.inum == 0) // inum为0，表示文件无效
        continue;
      memmove(p, de.name, DIRSIZ); //将读取到的文件名保存到buf末尾中
      p[DIRSIZ] = 0; //buf中每个文件名末尾+0，好输出
      if(stat(buf, &st) < 0){ // ??，判断保存 buf 是否正常
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size); //打印信息
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){ // 如果命令行只输入了ls ，则打印当前目录文件
    ls(".");
    exit(0);
  }
  for(i=1; i<argc; i++) // 给出了路径，则打印给出的路径目录文件
    ls(argv[i]);
  exit(0);
}
