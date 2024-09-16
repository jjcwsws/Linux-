#include "processbar.h"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define FILESIZE 1024*1024*1024

//模拟一种场景,表示一种下载的任务
void download(callback_t cb) // 回调函数的形式
{
    srand(time(NULL)^1023);
    int total = FILESIZE;
    while(total){
        usleep(10000); //下载动作
        int one = rand()%(1024*1024*10);
        total -= one;
        if(total < 0) total = 0;

        // 当前的进度是多少？
        int download = FILESIZE - total;
        double rate = (download*1.0/(FILESIZE))*100.0; // 0 23.4 35.6, 56.6
        cb(rate);
        //process_flush(rate);
        //printf("download: %f\n", rate); // rate出来了，应该让进度条刷新
    }
}

int main()
{
    //process();
    download(process_flush);
    return 0;
}





