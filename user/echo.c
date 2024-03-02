#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) // argc 为 终端输入的字符串个数，argv为指针数组，元素为指向不同的字符串的指针
{
  int i;

  for(i = 1; i < argc; i++)
  {
    write(1, argv[i], strlen(argv[i]));//write系统调用，从指针数组[1]开始把数据写入到标准输出，[0]号元素是echo，echo hello world  ，echo也是字符串的一个                         
    if(i + 1 < argc) // 输出后，如果此次不是最后一个要输出的字符串，则输出空格，隔开后续的字符串，如果是，则输出 换行符
    {
      write(1, " ", 1);
    }
    else
     {
      write(1, "\n", 1);
    }
  }
  exit(0); // 成功退出，返回0到wait（）
}
