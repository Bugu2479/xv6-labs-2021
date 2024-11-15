#define T_DIR     1   // Directory
#define T_FILE    2   // File
#define T_DEVICE  3   // Device

struct stat {  // 文件的状态信息
  int dev;     // File system's disk device ，文件系统磁盘是哪个？
  uint ino;    // Inode number ，每一个文件都有对应的inode，里面包含了与该文件有关的一些信息。
  short type;  // Type of file
  short nlink; // Number of links to file
  uint64 size; // Size of file in bytes
};
