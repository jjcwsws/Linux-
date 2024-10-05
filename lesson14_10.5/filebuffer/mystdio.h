#pragma once

#include <stdio.h>

#define SIZE 4096
#define NONE_FLUSH (1<<1) 
#define LINE_FLUSH (1<<2) 
#define FULL_FLUSH (1<<3) 

typedef struct _myFILE
{
    //char inbuffer[];
    char outbuffer[SIZE];
    int pos;
    int cap;
    int fileno;
    int flush_mode;
}myFILE;


myFILE *my_fopen(const char *pathname, const char *mode);
int my_fwrite(myFILE *fp, const char *s, int size);
//int my_fread();
void my_fflush(myFILE *fp);
void my_fclose(myFILE *fp);
void DebugPrint(myFILE *fp);
