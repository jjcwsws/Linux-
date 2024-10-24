#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <cassert>
#include <sys/wait.h>

void PrintSig(sigset_t &pending)
{
    std::cout << "Pending bitmap: ";
    for (int signo = 31; signo > 0; signo--)
    {
        if (sigismember(&pending, signo))
        {
            std::cout << "1";
        }
        else
        {
            std::cout << "0";
        }
    }
    std::cout << std::endl;
}

void handler(int signo)
{
    sigset_t pending;
    sigemptyset(&pending);
    int n = sigpending(&pending); // 我正在处理2号信号哦！！
    assert(n == 0);

    // 3. 打印pending位图中的收到的信号
    std::cout << "递达中...: ";
    PrintSig(pending); // 0: 递达之前，pending 2号已经被清0. 1: pending 2号被清0一定是递达之后
    std::cout << signo << " 号信号被递达处理..." << std::endl;
}

int main()
{
    // 对2号信号进行自定义捕捉 --- 不让进程因为2号信号而终止
    signal(2, handler);

    // 1. 屏蔽2号信号
    sigset_t block, oblock;
    sigemptyset(&block);
    sigemptyset(&oblock);
    sigaddset(&block, 2); // SIGINT --- 根本就没有设置进当前进程的PCB block位图中

    // 0. for test: 如果我屏蔽了所有信号呢？？？
    // for(int signo = 1; signo <= 31; signo++) // 9， 19号信号无法被屏蔽， 18号信号会被做特殊处理
    //     sigaddset(&block, signo); // SIGINT --- 根本就没有设置进当前进程的PCB block位图中
    // 1.1 开始屏蔽2号信号，其实就是设置进入内核中
    int n = sigprocmask(SIG_SETMASK, &block, &oblock);
    assert(n == 0);
    // (void)n; // 骗过编译器，不要告警，因为我们后面用了n，不光光是定义
    std::cout << "block 2 signal success" << std::endl;
    std::cout << "pid: " << getpid() << std::endl;
    int cnt = 0;
    while (true)
    {
        // 2. 获取进程的pending位图
        sigset_t pending;
        sigemptyset(&pending);
        n = sigpending(&pending);
        assert(n == 0);

        // 3. 打印pending位图中的收到的信号
        PrintSig(pending);
        cnt++;

        // 4. 解除对2号信号的屏蔽
        if (cnt == 20)
        {
            std::cout << "解除对2号信号的屏蔽" << std::endl;
            n = sigprocmask(SIG_UNBLOCK, &block, &oblock); // 2号信号会被立即递达, 默认处理是终止进程
            assert(n == 0);
        }
        // 我还想看到pending 2号信号 1->0 : 递达二号信号！
        sleep(1);
    }

    // 有没有涉及到将数据设置进内核中呢？没有！！！
    // sigset_t数据类型，int double float class没有差别
    // sigset_t s;  // 用户栈上开批了空间
    // sigemptyset(&s);
    // sigaddset(&s, 2);
    // pid_t id = fork();
    // if(id == 0)
    // {
    //     sleep(2);
    //     int a = 10;
    //     a /= 0; // 故意异常，收到SIGFPE-> core
    //     exit(0);
    // }

    // // father
    // int status = 0;
    // pid_t rid = waitpid(id, &status, 0);
    // if(rid > 0)
    // {
    //     std::cout << "exit code: " << ((status>>8) & 0xFF) << std::endl; // 没有意义
    //     std::cout << "exit signal: " << (status & 0x7F) << std::endl;
    //     std::cout << "core dump: " << ((status>>7) & 0x1) << std::endl;
    // }

    // std::cout << "run begin ..." << std::endl;
    // int a = 10;
    // a /= 0; // SIGFPE -> Core

    // std::cout << "run end ..." << std::endl;

    // while(true) sleep(1);
    return 0;
}