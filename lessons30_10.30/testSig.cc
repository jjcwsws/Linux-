#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>



// void handler(int signo)
// {
//     std::cout << "child quit, father get a signo: " << signo << std::endl;
// }

// void CleanupChild(int signo)
// {
//     // v1
//     // if (signo == SIGCHLD)
//     // {
//     //     pid_t rid = waitpid(-1, nullptr, 0);
//     //     if (rid > 0)
//     //     {
//     //         std::cout << "wait child success: " << rid << std::endl;
//     //     }
//     // }
//     // std::cout << "wait sub process done" << std::endl;
//     // v2
//     // if (signo == SIGCHLD)
//     // {
//     //     while (true)
//     //     {
//     //         pid_t rid = waitpid(-1, nullptr, 0); // -1 : 回收任意一个子进程
//     //         if (rid > 0)
//     //         {
//     //             std::cout << "wait child success: " << rid << std::endl;
//     //         }
//     //         else if(rid <= 0) break;
//     //     }
//     // }
//     // std::cout << "wait sub process done" << std::endl;
//     // v3
//     if (signo == SIGCHLD)
//     {
//         while (true)
//         {
//             pid_t rid = waitpid(-1, nullptr, WNOHANG); // -1 : 回收任意一个子进程
//             if (rid > 0)
//             {
//                 std::cout << "wait child success: " << rid << std::endl;
//             }
//             else if (rid <= 0)
//                 break;
//         }
//     }
//     std::cout << "wait sub process done" << std::endl;
// }

int main()
{
    // signal(SIGCHLD, CleanupChild);
    signal(SIGCHLD, SIG_IGN);

    // 50个退出，50个没有
    for (int i = 0; i < 100; i++)
    {
        pid_t id = fork();
        if (id == 0)
        {
            // child
            int cnt = 5;
            while (cnt--)
            {
                std::cout << "I am child process: " << getpid() << std::endl;
                sleep(1);
            }
            std::cout << "child process died" << std::endl;
            exit(0);
        }
    }

    // father
    while (true)
        sleep(1);
}

// void Print(sigset_t &pending)
// {
//     std::cout << "curr process pending: ";
//     for(int sig = 31; sig >= 1; sig--)
//     {
//         if(sigismember(&pending, sig)) std::cout << "1";
//         else std::cout << "0";
//     }
//     std::cout << "\n";
// }

// void handler(int signo)
// {
//     std::cout << "signal : " << signo << std::endl;
//     // 不断获取当前进程的pending信号集合并打印
//     sigset_t pending;
//     sigemptyset(&pending);
//     while(true)
//     {
//         sigpending(&pending);
//         Print(pending);
//         sleep(1);
//     }
// }

// int main()
// {
//     struct sigaction act, oact;
//     act.sa_handler = handler;
//     act.sa_flags = 0;
//     sigemptyset(&act.sa_mask); // ??
//     sigaddset(&act.sa_mask, 3);
//     sigaddset(&act.sa_mask, 4);
//     sigaddset(&act.sa_mask, 5);

//     sigaction(2, &act, &oact);

//     while(true) sleep(1);

//     return 0;
// }
