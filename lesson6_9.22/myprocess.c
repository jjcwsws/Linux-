#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


int main()
{
   // pid_t id = fork();
   // if(id == 0)
   // {
   //     fork();
   //     //child
   //     int cnt = 500;
   //     while(cnt)
   //     {
   //         printf("I am child, pid: %d, ppid: %d\n", getpid(), getppid());
   //         sleep(1);
   //         cnt--;
   //     }
   //     exit(0); // 让子进程直接退出
   // }
   // int cnt = 5;
   // // father
   // while(cnt)
   // {
   //     cnt--;
   //     printf("I am father, pid: %d, ppid: %d\n", getpid(), getppid());
   //     sleep(1);
   // }

//    wait(NULL);
//    printf("father wait child done...\n");
//    sleep(5);
//
    //printf("hello Linux\n");
    //sleep(10);
    //int a = 0;
    //scanf("%d", &a);
    //printf("a=%d\n", a);

    //return 0;

    while(1)
    {
        printf("hello process, pid: %d\n", getpid());
        sleep(1);
    }
}
