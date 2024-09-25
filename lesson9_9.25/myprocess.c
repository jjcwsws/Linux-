#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    char *const env[] ={
        (char*)"haha=hehe",
        (char*)"PATH=/",
        NULL
    };
    printf("I am a process, pid: %d\n", getpid());
    //putenv("MYVAL=bbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    pid_t id = fork();
    if(id == 0)
    {
        extern char**environ;
        sleep(1);
        execle("./mytest", "mytest", NULL, environ); // 我们传递环境变量表了吗？？no. 子进程默认就拿到了.他是怎么做到的？
        //execle("./mytest", "mytest", NULL, env); // 我们传递环境变量表了吗？？no. 子进程默认就拿到了.他是怎么做到的？
        //execl("/usr/bin/python3", "python3", "test.py", NULL);
        //execl("/usr/bin/bash", "bash", "test.sh", NULL);
        //execl("./mytest", "mytest", NULL); // 我们传递环境变量表了吗？？no. 子进程默认就拿到了.他是怎么做到的？
        //char *const argv[] = {
        //    (char*)"ls",
        //    (char*)"-a",
        //    (char*)"-l"
        //};
        //sleep(3);
        //printf("exec begin...\n");
        //execvp("ls", argv);
        //execv("/usr/bin/ls", argv);
        //execl("/usr/bin/ls", "ls", "-a", "-l", NULL); //NULL 不是 "NULL"
        //execlp("ls", "ls", "-a", "-l", NULL); //NULL 不是 "NULL"
        //execl("/usr/bin/top", "/usr/bin/top", NULL); //NULL 不是 "NULL"
        printf("exec end ...\n");
        exit(1);
    }

    pid_t rid = waitpid(id, NULL, 0);
    if(rid > 0)
    {
        printf("wait success\n");
    }
    
    exit(1);
}








































//#define NUM 5
//
//typedef void(*fun_t)();
//fun_t tasks[NUM];
////////////////////////////任务///////////////////////////////
//void printLog()
//{
//    printf("this is a log print task\n");
//}
//
//void printNet()
//{
//    printf("this is a net task\n");
//}
//
//void printNPC()
//{
//    printf("this is a flush NPC\n");
//}
//
//
//void initTask()
//{
//    tasks[0] = printLog;
//    tasks[1] = printNet;
//    tasks[2] = printNPC;
//    tasks[3] = NULL;
//}
//
//void excuteTask()
//{
//    for(int i = 0; tasks[i]; i++) tasks[i](); // 回调机制
//}
//
//int main()
//{
//    initTask();
//    pid_t id = fork();
//    if(id == 0)
//    {
//        //int a = 10;
//        // child
//        int cnt = 5;
//        while(cnt)
//        {
//            printf("I am child process, pid: %d, ppid: %d, cnt: %d\n", getpid(), getppid(), cnt);
//            cnt--;
//            sleep(1);
//
//           // int *p = NULL;
//           // *p = 100;
//            //a/=0;
//        }
//        sleep(1);
//        exit(111);
//    }
//
//    int status = 0;
//    while(1)
//    {
//        pid_t rid = waitpid(id, &status, WNOHANG);
//        if(rid > 0)
//        {
//            printf("wait success, rid: %d, status: %d, exit code: %d\n", rid, status, WEXITSTATUS(status));
//            break;
//        }
//        else if(rid == 0)
//        {
//            printf("father say: child is running, do other thing\n");
//            printf("##############task begin#####################\n");
//            excuteTask();
//            printf("##############task end  #####################\n");
//        }
//        else
//        {
//            perror("waitpid");
//            break;
//        }
//
//        sleep(1);
//    }
//
//
//    //int status = 0;
//    //pid_t rid = waitpid(id, &status, 0); // 为什么非得用系统调用？
//    //if(rid >0)
//    //{
//    //    // status & 0x7F: 0000 0000 0000 0000 0000 0000 0111 1111
//    //    // printf("wait success, rid: %d, status: %d, exit signo: %d, exit code: %d\n", rid, status, status&0x7F, (status>>8)&0xFF);
//    //    if(WIFEXITED(status))
//    //    {
//    //        printf("wait success, rid: %d, status: %d, exit code: %d\n", rid, status, WEXITSTATUS(status));
//    //    }
//    //    else{
//    //        printf("child process error!\n");
//    //    }
//    //}
//
//    return 0;
//}
