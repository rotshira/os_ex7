#include "fs.h"
//1
int find_empty_block()
{
    int k;
    for (int i = 0; i < sb.num_blocks; i++)
    {
        k=dbs[i].next_block_num;
        if (k == (-1))
            return i;
    }
    return -1;
}
//2
int find_empty_inode()
{
    int k;
    for (int i = 0; i < sb.num_inodes; i++)
    {
        k = inodes[i].first_block;
        if (k == (-1))
            return i;
    }
    return (-1);
}
//2
void mymkfs(int size)
{
    int ufSize = size - sizeof(superblock); // size of ufs
    sb.num_inodes = (ufSize / 10) / sizeof(inode);
    sb.num_blocks = (ufSize - sb.num_inodes) / sizeof(disk_block);
    sb.size_blocks = BLOCKSIZE;
    int i = 0;
    inodes = malloc(sizeof(inode) * sb.num_inodes);
    dbs = malloc(sizeof(disk_block) * sb.num_blocks);
    for (i = 0; i < sb.num_inodes; i++)
    {
        inodes[i].size = (-1);
        inodes[i].first_block = (-1);
        strcpy(inodes[i].name, "");
    }
    for (i = 0; i < sb.num_blocks; i++) {// initialization
        dbs[i].next_block_num = (-1);
    }
    int storage = make_file(sizeof(struct mydirent), "folder");
    inodes[storage].dir = 1;
    struct mydirent *dir_current = malloc(sizeof(struct mydirent));
    if (dir_current == NULL)
    {
        return;
    }

    dir_current->size = 0;
    for (i = 0; i < MAX_DIR; i++)
        dir_current->fds[i] = (-1);

    strcpy(dir_current->d_name, "file");
    char *nowDir = (char *)dir_current;
    for (i = 0; i < sizeof(struct mydirent); i++)
        writeText(storage, i, nowDir[i]);
    free(dir_current);
}
//3
int mymount(const char *source, const char *target, const char *filesystemtype, unsigned long mountflags, const void *data)
{
    if (target != NULL)
    {
        FILE *myfile = fopen("fs_data", "w+");
        fwrite(&sb, sizeof(superblock), 1, myfile);
        fwrite(inodes, sizeof(inode), sb.num_inodes, myfile);
        fwrite(dbs, sizeof(disk_block), sb.num_blocks, myfile);
        fclose(myfile);
    }
    if (source != NULL)
    {
        FILE *myfile = fopen(source, "r");
        fread(&sb, sizeof(struct superblock), 1, myfile);
        inodes = malloc(sizeof(struct inode) * sb.num_inodes);
        dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);
        fread(inodes, sizeof(struct inode), sb.num_inodes, myfile);
        fread(dbs, sizeof(struct disk_block), sb.num_blocks, myfile);
        fclose(myfile);
    }
}


//4
int r_b_help(int filenum){
int rb = inodes[filenum].first_block;
return rb;
}

void writeText(int filenum, int _pos, char data)
{
    int rb = r_b_help(filenum);
    int position = _pos;
    while (position >= BLOCKSIZE)
    {
        position -= BLOCKSIZE;
        if (dbs[rb].next_block_num == -2)
        {
            dbs[rb].next_block_num = find_empty_block();
            rb = dbs[rb].next_block_num;
            dbs[rb].next_block_num = -2;
        }
        else
        {
            rb = dbs[rb].next_block_num;
        }
    }

    if (_pos > inodes[filenum].size)
        inodes[filenum].size = _pos + 1;
    dbs[rb].data[position] = data;
}
//5
struct mydirent *myread_dir(int fd)
{

    if (inodes[fd].dir != 1)
    {
        perror("error");
    }
    return (mydirent *)dbs[inodes[fd].first_block].data;
}
//6
int d_b_help(int f_d){
int d_b = inodes[f_d].first_block;
return d_b;
}

int open_tmp1(const char *root)
{
     char buffer[BUFF_SIZE];
     char *select;
     strcpy(buffer, root);
     const char x[2] = "/";
     select = strtok(buffer,x);
     char now[NAME_SIZE] = "";
     char last[NAME_SIZE] = "";
     while (select != NULL)
    {
        strcpy(last, now); 
        strcpy(now, select);
        select = strtok(NULL, x);
    }
    int i = 0;
    int limit = sb.num_inodes;
    for ( i = 0; i < limit; i++)
    {
        char tmp = inodes[i].name;
        if (!strcmp(tmp, now)) 
            return i;
        
    }
    int f_d = open_tmp1(last);
    int d_b = d_b_help(f_d);
    mydirent *c_dir = (mydirent *)dbs[d_b].data;
    int dir = make_file(sizeof(mydirent), now);
    c_dir->fds[c_dir->size++] = dir;
    inodes[dir].dir = 1;
    mydirent *n_dir = malloc(sizeof(mydirent));
    n_dir->size = 0;
    for (size_t i = 0; i < MAX_DIR; i++) 
       n_dir->fds[i] = -1;
    char *t_dir = (char *)n_dir;
    for (size_t i = 0; i < sizeof(mydirent); i++)
        writeText(dir, i,t_dir[i]);
    strcpy(n_dir->d_name, now);
    return dir;
}
//7
int open_tmp2(const char *path, const char *name)
{
    int f_d = make_file(1, name);
    int mydir = open_tmp1(path);
    mydirent *c_dir = myread_dir(mydir);
    c_dir->fds[c_dir->size++] = f_d;
    return f_d;
}
//8

int myopen(const char *pathname, int flags) // how i want to open the file. flags = r/w/a.
{
    char data[BUFF_SIZE];
    strcpy(data, pathname);
    char *select;
    const char x[2] = "/";
    select = strtok(data, x);
    char now[NAME_SIZE] = "";
    char last[NAME_SIZE] = "";
    while (select != NULL)
    {
        strcpy(last,  now); 
        strcpy( now, select);
        select = strtok(NULL, x);
    }
    int i ;
    for ( i = 0; i < sb.num_inodes; i++)
    {
        if (!strcmp(inodes[i].name,now))
        {
            open_f[i].fd = i;
            open_f[i].pos = 0;
            return i;
        }
    }
    int index = open_tmp2(last,  now);
    open_f[index].fd = i;
    open_f[index].pos = 0;
    return index;
}
//9
int make_file(int size, const char *name)
{
    int i_node = find_empty_inode();
    int current_block = find_empty_block();
    inodes[i_node].size = size;
    inodes[i_node].first_block = current_block;
    dbs[current_block].next_block_num = -2;
    strcpy(inodes[i_node].name, name);
    if (size > BLOCKSIZE)
    {
        int fix_size = -(3 * BLOCKSIZE) / 4;
        int nextBlock = 0;
        while (fix_size < size)
        {
           nextBlock = find_empty_block();
            dbs[current_block].next_block_num = nextBlock;
           current_block = nextBlock;
            fix_size += BLOCKSIZE;
        }
    }
    dbs[current_block].next_block_num = -2;
    return i_node;
}
//10
int myclose(int myfd)
{
    open_f[myfd].fd = -1;
    open_f[myfd].pos = -1;
}
// 11
int n_b(int b)
        {
            b = dbs[b].next_block_num;
            return b;
        }
int pos_h(int pos,int myfd)
        {
            pos = open_f[myfd].pos;
            return pos;
        }

ssize_t myread(int myfd, void *buf, size_t count)
{
    if (inodes[myfd].dir == 1 || open_f[myfd].fd != myfd)
    {
        perror("empty");
        return -1;
    }
    char *text = malloc(count + 1);
    for (int i = 0; i < count; i++){
        int pos = pos_h(pos,myfd);
        int b = inodes[myfd].first_block;
        while (pos >= BLOCKSIZE) {
            pos = pos - BLOCKSIZE;
            b = n_b(b);
        }
        text[i] = dbs[b].data[pos];
        open_f[myfd].pos++;
    }
    text[count] = '\0';
    strncpy(buf, text, count);
    free(text);
    return open_f[myfd].pos; 

}

//12
ssize_t mywrite(int myfd, const void *buf, size_t count)
{
    char *buff = (char *)buf;
    for (int i = 0; i < count; i++)
    {
        writeText(myfd, open_f[myfd].pos, buff[i]);
        open_f[myfd].pos++;
    }
    return open_f[myfd].pos;
}
//13
off_t mylseek(int myfd, off_t offset, int whence)
{
     if (whence == SEEK_CUR)
        open_f[myfd].pos = open_f[myfd].pos + offset;
    else if (whence == SEEK_SET)
        open_f[myfd].pos = offset;
    else if (whence == SEEK_END)
        open_f[myfd].pos = inodes[myfd].size + offset;
    if (open_f[myfd].pos < 0)
        open_f[myfd].pos = 0;
    return open_f[myfd].pos;
}

//14
myDIR *myopendir(const char *name)
{
    int route = open_tmp1(name);
    myDIR *txt = (myDIR *)malloc(sizeof(myDIR));
    txt->path = route;
    return txt;
}
//15
struct mydirent *myreaddir(myDIR *dirp)
{
    mydirent *txt = (mydirent *)malloc(sizeof(mydirent));
    txt = (mydirent*)dbs[inodes[dirp->path].first_block].data;
    return txt;
}
//16
int myclosedir(myDIR *dirp)
{
    printf("Dir closed");
    return 0;
}

