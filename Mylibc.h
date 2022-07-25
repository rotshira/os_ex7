#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


typedef struct myFILE
{
    int file_number;
    char* data;
    int size;
    char kind[3];
    int pos;
}myFILE;


myFILE *myfopen(const char *restrict pathname, const char *mode);

int myfclose(myFILE *stream);

size_t myfread(void *restrict ptr, size_t size, size_t nmemb, myFILE *stream);

size_t myfwrite(const void *restrict ptr, size_t size, size_t nmemb, myFILE *stream);

int myfseek(myFILE *stream, long offset, int whence);

int myfscanf(myFILE *restrict stream, const char *restrict format, ...);

int myfprintf(myFILE *restrict stream, const char *restrict format, ...);