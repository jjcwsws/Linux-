#include "mystdio.h"
#include <string.h>
#include <unistd.h>

const char *filename = "./log.txt";

int main()
{
    myFILE *fp = my_fopen(filename, "w");
    if(fp == NULL) return 1;

    int cnt = 5;
    char buffer[64];
    while(cnt)
    {
        snprintf(buffer, sizeof(buffer), "helloworld,hellobit,%d  ", cnt--);
        my_fwrite(fp, buffer, strlen(buffer));
        DebugPrint(fp);
        sleep(2);
        //my_fflush(fp);
    }

    fork();

    my_fclose(fp);
    return 0;
}




//#include <stdio.h>
//#include <unistd.h>
//#include <string.h>
//
//int main()
//{
//    // 使用system call
//    const char *s1 = "hello write\n";
//    write(1, s1, strlen(s1));
//
//    // 使用C语言接口
//    const char *s2 = "hello fprintf\n";
//    fprintf(stdout, "%s", s2);
//
//    const char *s3 = "hello fwrite\n";
//    fwrite(s3, strlen(s3), 1, stdout);
//
//
//    fork();
//    return 0;
//}
