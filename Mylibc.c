#include "Mylibc.h"
#include "fs.h"

myFILE* myfopen(const char *pathname, const char *mode) 
{
    int file_number = myopen(pathname, 0);
    if (file_number== (-1)) 
    {  
        return NULL;
    }

    myFILE* m_file = (myFILE*)malloc(sizeof(myFILE));// allocate myFILE 
    if(mode[1] == '+')
    {
        m_file->kind[0] = mode[0];
        m_file->kind[1] = mode[1];
    }
    else
     m_file->kind[0] = mode[0];
    
    m_file->kind[3] = '\0';
    m_file->file_number = file_number;
    int s = inodes[file_number].size;
    m_file->size = s;
    if (!strcmp(mode, "a")) 
    { 
        int k = inodes[file_number].size;
        m_file->pos = k;
    } 
    else 
      m_file->pos = 0;
      char c =  malloc(m_file->size);
    m_file->data = c;
    if (!strcmp(mode, "w")) 
    { 
        int i = 0;
        int lim =  m_file->size;
        while(i< lim){
            m_file->data[i++] = ' ';
        }
        char size_m = m_file->size;
        m_file->data[size_m] = '\0';
    } 
    else    
       m_file->data[m_file->size+1] = '\0';
    
    myclose(file_number);
    return m_file;
}


int myfclose(myFILE *stream){
    char temp1 = stream->kind[0];
    if(temp1 != 'r'){
        free(stream->data);
        free(stream);
    }else
    {
        int i;
        int temp2 = stream->size; 
        while(i < temp2)
        {
            writeText(stream->file_number, i, stream->data[i]);
            i++;
        }
        free(stream->data);
        free(stream);
    }
    return 0;

}

size_t myfread(void *restrict ptr, size_t size, size_t nmemb, myFILE *stream)
{
    int byte_need = size * nmemb;
    char *txt = malloc(byte_need + 1);
    if(txt == NULL)
    {
        perror("res has faild!");
        return 1;
    }
    int pos = stream->pos;
    int i = 0;
    while (i < byte_need)
    {
        int x = stream->size;
        if (stream->pos+i < x) 
        {
            int temp = stream->data[stream->pos+i];
            txt[i] = temp;
            pos++;
        }
        i++;
    }
    
    txt[byte_need] = '\0';
    stream->pos = pos;
    strncpy(ptr,txt, byte_need); 
    free(txt);
    size_t ans = stream -> pos;
    return ans;
}

size_t myfwrite(const void *restrict ptr, size_t size, size_t nmemb, myFILE *stream)
{
    size_t byte_there = size * nmemb;
    char *txt = (char*)ptr;
    if(stream->pos+byte_there > stream->size){
        char* buffer = malloc(stream->size+1);
        int i = 0;
        while(i < stream->size){
             buffer[i] = stream->data[i];
            i++;
        }
        free(stream->data);
        char data_t = malloc(stream->pos + byte_there);
        stream->data = data_t;
         i = 0;
        while(i < stream->size){
            char data_t = buffer[i];
            stream->data[i] = data_t;
            i++;
        }
        free(buffer);
    }
    
    int ptr_T = stream->pos;
    int i = 0;
    while(i < byte_there){
        char data_p =txt[i];
        stream->data[stream->pos+i] = data_p;
        ptr_T++;
        i++;
    }
    stream->pos = ptr_T;
    size_t ans = stream->pos;
    return ans;
}


int myfseek(myFILE *stream, long offset, int whence)
{
     if(whence == SEEK_CUR) 
    
        stream->pos += offset;
    
    else if(whence == SEEK_SET) 
    
        stream->pos = offset;
    
    else if (whence == SEEK_END) 
    
        stream->pos = stream->size+offset;
    int ans = stream->pos;
    return ans;
}

int myfscanf(myFILE  *stream, const char *format, ...)
{
    int i;
    va_list arguments;
    va_start(arguments, format);
    int* sum = va_arg(arguments, void*);  
    char* c = va_arg(arguments, void*);
    float* f = va_arg(arguments, void*);
    i = 0;
    while(i < (int)strlen(format)){
        if (format[i] == '%')
        {
            if(format[i + 1] == 'd'){
                sum = va_arg(arguments, void*);
                myfread(&sum, sizeof(int), 1, stream);
            }
            if(format[i + 1] == 'f'){
                f = va_arg(arguments, void*);
                myfread(&f, sizeof(float), 1, stream);
            }
            if (format[i + 1] == 'c'){
                c = va_arg(arguments, void*);
                myfread(&c, sizeof(char), 1, stream);
            }
        }
        i++;
    }
    int ans =i;
    return ans;
}

int myfprintf(myFILE *restrict stream, const char *restrict format, ...)
{
    int i = 0;
    va_list arguments;                     
    va_start(arguments, format);    
    int* sum = va_arg(arguments, void*);  
    char* c = va_arg(arguments, void*);
    float* f = va_arg(arguments, void*);
    char* trash = va_arg(arguments, void*);
    while(i < (int)strlen(format)){
        if(format[i] == '%')
        {
            if(format[i + 1] == 'd')
            {
                sum = va_arg(arguments, void*);
                myfwrite(&sum, sizeof(int), 1, stream);
            }

            if(format[i + 1] == 'f')
            {
                f = va_arg(arguments, void*);
                myfwrite(&f, sizeof(float), 1, stream);
             }

            if(format[i + 1] == 'c')
            {
                c = va_arg(arguments, void*);
                myfwrite(&c, sizeof(char), 1, stream);
            }
        }
        else{
            for(; (format[i] != '%'); i++)
            {
                trash = va_arg(arguments, void*);
                myfwrite(&trash, sizeof(char), 1, stream);
            }
        }
        i++;
    }
    int ans = i ;
    return ans;
}