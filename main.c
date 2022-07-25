#include "fs.h"
#include "Mylibc.h"
// shira and amit
int main(int argc, char const *argv[])
{
    mymkfs(10000);
    int x = myopen("start", 1); 
    mywrite(x,"Reading and writing test!!!",28);
    char my_buf[100];
    myread(x, my_buf, 30);
    printf("%s\n",my_buf);
    myclose(x);
    printf("Succeeded!\n");
    return 0;
}