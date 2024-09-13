#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int main()
{
    printf("我的pid: %d\n", getpid());
    printf("我的父进程pid: %d\n", getppid());
}
