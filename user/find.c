#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *object)
{
    printf("i will search %s in this path: %s \n",object,path);
}
int main(int argc, char *argv[])
{
    if(argc <= 1)
    {
       printf("find not have argument :( \n");
       exit(0);
    }
    if(argc == 2)
    {
        find(".","a");
        exit(0);
    }
    if(argc > 3)
    {
        printf("to much more argument !! \n");
        exit(0);
    }
    find(argv[1],argv[2]);
    exit(0);
}