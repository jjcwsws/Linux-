#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// volatile int g_flag = 0;
int g_flag = 0;

void changeflag(int signo)
{
    (void)signo;
    printf("将g_flag,从%d->%d\n", g_flag, 1);
    g_flag = 1;
}

int main()
{
    signal(2, changeflag);

    while(!g_flag); // 故意写成这个样子, 编译器默认会对我们的代码进行自动优化！
    // {
    //     printf("hello world\n");
    //     sleep(1);
    // }

    printf("process quit normal\n");
    return 0;
}