#include "mystdio.h"
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

const char *toString(int flag)
{
    if(flag & NONE_FLUSH) return "None";
    else if(flag & LINE_FLUSH) return "Line";
    else if(flag & FULL_FLUSH) return "FULL";
    return "Unknow";
}

void DebugPrint(myFILE *fp)
{
    printf("outbufer: %s\n", fp->outbuffer);
    printf("fd: %d\n", fp->fileno);
    printf("pos: %d\n", fp->pos);
    printf("cap: %d\n", fp->cap);
    printf("flush_mode: %s\n", toString(fp->flush_mode));
}

myFILE *my_fopen(const char *pathname, const char *mode)
{
    int flag = 0;
    if(strcmp(mode, "r") == 0)
    {
        flag |= O_RDONLY;
    }
    else if(strcmp(mode, "w") == 0)
    {
        flag |= (O_CREAT|O_WRONLY|O_TRUNC);
    }
    else if(strcmp(mode, "a") == 0)
    {
        flag |= (O_CREAT|O_WRONLY|O_APPEND);
    }
    else
    {
        return NULL;
    }

    int fd = 0;
    if(flag & O_WRONLY)
    {
        umask(0);
        fd = open(pathname, flag, 0666);
    }
    else
    {
        fd = open(pathname, flag);
    }
    if(fd < 0) return NULL;

    myFILE *fp = (myFILE*)malloc(sizeof(myFILE));
    if(fp == NULL) return NULL;
    fp->fileno = fd;
    fp->cap = SIZE;
    fp->pos = 0;
    fp->flush_mode = LINE_FLUSH;

    return fp;
}

void my_fflush(myFILE *fp)
{
    if(fp->pos == 0) return;
    write(fp->fileno, fp->outbuffer, fp->pos);
    fp->pos = 0;
}

int my_fwrite(myFILE *fp, const char *s, int size)
{
    // 1. 写入
    memcpy(fp->outbuffer+fp->pos, s, size);
    fp->pos += size;

    // 2. 判断，是否要刷新
    if((fp->flush_mode & LINE_FLUSH) && fp->outbuffer[fp->pos-1] == '\n')
    {
        my_fflush(fp);
    }
    else if((fp->flush_mode & FULL_FLUSH) && fp->pos  == fp->cap)
    {
        my_fflush(fp);
    }
    return size;
}

void my_fclose(myFILE *fp)
{
    my_fflush(fp);
    close(fp->fileno);
    free(fp);
}

















