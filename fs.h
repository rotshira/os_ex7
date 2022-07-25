#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_DIR 10
#define BUFF_SIZE 80
#define PATH_MAX 128
#define BLOCKSIZE 512
#define NAME_SIZE 9

typedef struct myopenfile
{
    int fd;
    int pos;
}myopenfile;

typedef struct superblock
{
    int num_inodes;
    int num_blocks;
    int size_blocks;
}superblock;

typedef struct inode
{
    int size;
    int first_block;
    char name[NAME_SIZE+1];
    int dir; 
}inode;

typedef struct disk_block
{
    int next_block_num;
    char data[BLOCKSIZE];
}disk_block;

typedef struct mydirent { 
    int size;
    int fds[MAX_DIR];
    char d_name[NAME_SIZE];
}mydirent;

typedef struct myDIR { 
    int path;
}myDIR;

myopenfile open_f [10000];
superblock sb;
inode *inodes;
disk_block *dbs;



void mymkfs(int size);
int mymount(const char *source, const char *target,const char *filesystemtype, unsigned long mountflags, const void *data);
int myopen(const char *pathname, int flags);
int myclose(int myfd);
ssize_t myread(int myfd, void *buf, size_t count);
ssize_t mywrite(int myfd, const void *buf, size_t count);
off_t mylseek(int myfd, off_t offset, int whence);
myDIR *myopendir(const char *name);
struct mydirent *myreaddir(myDIR *dirp);
int myclosedir(myDIR *dirp);
void writeText(int filenum, int _pos, char data);
int make_file(int size,const char* name); 
struct mydirent *myread_dir(int fd);
int open_tmp1(const char *pathname);
int open_tmp2(const char *path, const char *name);
int r_b_help(int filenum);
int n_b(int b);

