#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"  

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){     //如果命令行参数小于2（rm + 文件名）,则不知道删除什么报错
    fprintf(2, "Usage: rm files...\n");
    exit(1);
  }

  for(i = 1; i < argc; i++){ 
    if(unlink(argv[i]) < 0){  // 删除文件，若删除失败（-1），则打印错误
      fprintf(2, "rm: %s failed to delete\n", argv[i]);
      break;
    }
  }

  exit(0);
}
