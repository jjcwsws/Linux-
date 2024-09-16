#include "processbar.h"
#include <string.h>
#include <unistd.h>

const char *lable="|/-\\";

//version 1
void process()
{
//    const int n = 101;
//    char buffer[n]; // 这块是编译不过的
    char buffer[NUM];
    memset(buffer, '\0', sizeof(buffer));
    int cnt = 0;
    int n = strlen(lable);
    buffer[0] = Head;
    while(cnt <= 100)
    {
        printf("[%-100s][%3d%%][%c]\r", buffer, cnt, lable[cnt%n]);
        fflush(stdout);
        buffer[cnt++] = Body;
        if(cnt < 100) buffer[cnt] = Head;
        usleep(50000);
    }

    printf("\n");
}

//version 2: 进度是多少，你进度条能知道吗？另外，什么进度？依附于其他应用的，比如下载
char buffer[NUM] = {0};
void process_flush(double rate)
{
    static int cnt = 0;
    int n = strlen(lable);
    if(rate <= 1.0) buffer[0] = Head;
    printf("[\033[4;32;44m%-100s\033[0m][%.1f%%][%c]\r", buffer, rate, lable[cnt%n]);
    fflush(stdout);

    buffer[(int)rate] = Body;
    if((int)rate+1 < 100) buffer[(int)(rate+1)] = Head;
    if(rate>=100.0) printf("\n");

    cnt++;
    cnt%=n;
}

























