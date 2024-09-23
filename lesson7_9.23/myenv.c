#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

void Print()
{
    printf("hello Print\n");
    //_exit(4);
}


int main()
{
    printf("hello Linux, hello bite");

    sleep(3);

    _exit(1);




//    while(1)
//    {
//        printf("I am a process: %d\n", getpid());
//        sleep(1);
//        Print();
//        _exit(3); // exit终止进程, status:进程退出的时候，退出码
//    }
}


//enum{
//    success=0,
//    open_err,
//    malloc_err
//};
//
//const char* errorToDesc(int code)
//{
//    switch(code)
//    {
//        case success:
//            return "success";
//        case open_err:
//            return "file open error";
//        case malloc_err:
//            return "malloc error";
//        default:
//            return "unknown error";
//    }
//}
//
//int Print()
//{
//    printf("hello Linux\n");
//    printf("hello Linux\n");
//    printf("hello Linux\n");
//    printf("hello Linux\n");
//
//    return 0;
//}
//
//int main()
//{
//    int *p = NULL;
//    while(1)
//    {
//        printf("process is running, pid: %d\n\n", getpid());
//        sleep(1);
//
//        *p = 100;
//    }
////    FILE *fp = fopen("./log.txt", "r");
////    printf("%d:%s\n", errno, strerror(errno));
////
//    //int n = Print();
//    //printf("n=%d\n", n);
//    //int code = malloc_err;
//    //printf("%s\n", errorToDesc(code));
//    //for(int i = 0; i < 200; i++)
//    //{
//    //    printf("%d: %s\n", i, strerror(i));
//    //}
//    //const char *str = "hello Linux";
//    //*str = 'H';
//    //printf("hello Linux\n");
//    return errno;
//}
//
////int g_val = 100;
////
////int main()
////{
////    pid_t id = fork();
////    if(id == 0)
////    {
////        int cnt = 0;
////        //子进程
////        while(1)
////        {
////            printf("child, pid: %d, ppid: %d, g_val: %d, &g_val: %p\n", getpid(), getppid(), g_val, &g_val);
////            sleep(1);
////            cnt++;
////            if(cnt == 5)
////            {
////                g_val = 200;
////                printf("child change g_val: 100->200\n");
////            }
////        }
////    }
////    else
////    {
////        while(1)
////        {
////            printf("father, pid: %d, ppid: %d, g_val: %d, &g_val: %p\n", getpid(), getppid(), g_val, &g_val);
////            sleep(1);
////        }
////
////    }
////}
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
//////int g_unval;
//////int g_val = 100;
//////
//////int main(int argc, char *argv[], char *env[])
//////{
//////    printf("code addr: %p\n", main);
//////    printf("init data addr: %p\n", &g_val);
//////    printf("uninit data addr: %p\n", &g_unval);
//////
//////    char *heap = (char*)malloc(20);
//////    char *heap1 = (char*)malloc(20);
//////    char *heap2 = (char*)malloc(20);
//////    char *heap3 = (char*)malloc(20);
//////    static int c;
//////    printf("heap addr: %p\n", heap);
//////    printf("heap1 addr: %p\n", heap1);
//////    printf("heap2 addr: %p\n", heap2);
//////    printf("heap3 addr: %p\n", heap3);
//////
//////    printf("stack addr: %p\n", &heap);
//////    printf("stack addr: %p\n", &heap1);
//////    printf("stack addr: %p\n", &heap2);
//////    printf("stack addr: %p\n", &heap3);
//////    printf("c addr: %p, c: %d\n", &c, c);
//////
//////    for(int i = 0; argv[i]; i++)
//////    {
//////        printf("argv[%d]=%p\n", i, argv[i]); //?
//////    }
//////    for(int i = 0; env[i]; i++)
//////    {
//////        printf("env[%d]=%p\n", i, env[i]);
//////    }
//////
//////    return 0;
//////}
//////
////////int main()
////////{
////////    const char *myenv=getenv("hello");
////////    if(myenv==NULL) printf("getenv get null\n");
////////    else printf("myenv=%s\n", myenv);
////////
////////
////////   // extern char **environ;
////////   // // environ
//////   // for(int i = 0; environ[i]; i++)
//////   // {
//////   //     printf("environ[%d]: %s\n", i, environ[i]);
//////   // }
//////
//////    return 0;
//////}
