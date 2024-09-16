#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX 101
#define LABLE '='

int main()
{
    char bar[MAX];
    memset(bar, '\0', sizeof(bar));

    int i = 0;
    while(i<=100)
    {
        printf("%s\r", bar);
        fflush(stdout);
        bar[i++] = LABLE;
        usleep(100000);
    }

    printf("\n");

    //int cnt = 10;
    //while(cnt>=0)
    //{
    //    printf("%-2d\r", cnt);
    //    fflush(stdout);
    //    cnt--;
    //    sleep(1);
    //}
    //printf("\n");
    return 0; // 程序就开始结束
}
