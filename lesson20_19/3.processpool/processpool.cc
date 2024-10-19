#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <ctime>
#include <sys/wait.h>
#include "task.hpp"

using namespace std;

enum
{
    UsageError = 1,
    ArgError,
    PipeError
};

void Usage(const std::string &proc)
{
    cout << "Usage: " << proc << " subprocess-num" << endl;
}

class Channel
{
public:
    Channel(int wfd, pid_t sub_id, const std::string &name)
        : _wfd(wfd), _sub_process_id(sub_id), _name(name)
    {
    }
    void PrintDebug()
    {
        cout << "_wfd: " << _wfd;
        cout << ",_sub_process_id: " << _sub_process_id;
        cout << ", _name: " << _name << endl;
    }
    string name() { return _name; }
    int wfd() { return _wfd; }
    pid_t pid() { return _sub_process_id; }
    void Close() { close(_wfd); }
    ~Channel()
    {
    }

private:
    int _wfd;
    pid_t _sub_process_id;
    string _name;
};

class ProcessPool
{
public:
    ProcessPool(int sub_process_num) : _sub_process_num(sub_process_num)
    {
    }
    int CreateProcess(work_t work) // 回调函数
    {
        std::vector<int> fds;
        for (int number = 0; number < _sub_process_num; number++)
        {
            int pipefd[2]{0};
            int n = pipe(pipefd);
            if (n < 0)
                return PipeError;
            pid_t id = fork();
            if (id == 0)
            {
                if (!fds.empty())
                {
                    std::cout << "close w fd: ";
                    for (auto fd : fds)
                    {
                        close(fd);
                        std::cout << fd << " ";
                    }
                    std::cout << std::endl;
                }
                // sleep(1);
                // child -> r
                close(pipefd[1]);
                // 执行任务
                dup2(pipefd[0], 0);
                work(pipefd[0]);
                exit(0);
            }

            // sleep(2);
            string cname = "channel-" + to_string(number);
            // father
            close(pipefd[0]);
            channels.push_back(Channel(pipefd[1], id, cname));

            // 把父进程的wfd保存
            fds.push_back(pipefd[1]);
        }
        return 0;
    }
    int NextChannel()
    {
        static int next = 0;
        int c = next;
        next++;
        next %= channels.size();
        return c;
    }
    void SendTaskCode(int index, uint32_t code)
    {
        cout << "send code: " << code << " to " << channels[index].name() << " sub prorcess id: " << channels[index].pid() << endl;
        write(channels[index].wfd(), &code, sizeof(code));
    }
    // 让子进程全部退出，只需要关闭所有的Channel w即可！
    void KillAll()
    {
        for (auto &channel : channels)
        {
            channel.Close();
            pid_t pid = channel.pid();

            pid_t rid = waitpid(pid, nullptr, 0);
            if (rid == pid)
            {
                std::cout << "wait sub process: " << pid << " success..." << std::endl;
            }
            std::cout << channel.name() << " close done" << " sub process quit now : " << channel.pid() << std::endl;
        }
    }
    void Wait()
    {
        // for (auto &channel : channels)
        // {
        //     pid_t pid = channel.pid();
        //     pid_t rid = waitpid(pid, nullptr, 0);
        //     if (rid == pid)
        //     {
        //         std::cout << "wait sub process: " << pid << " success..." << std::endl;
        //     }
        // }
    }
    void Debug()
    {
        for (auto &channel : channels)
        {
            channel.PrintDebug();
        }
    }
    ~ProcessPool()
    {
    }

private:
    int _sub_process_num;
    vector<Channel> channels;
};

void CtrlProcessPool(ProcessPool *processpool_ptr, int cnt)
{
    while (cnt)
    {
        // a. 选择一个进程和通道
        int channel = processpool_ptr->NextChannel();
        // cout << channel.name() << endl;

        // b. 你要选择一个任务
        uint32_t code = NextTask();

        // c. 发送任务
        processpool_ptr->SendTaskCode(channel, code);

        sleep(1);
        cnt--;
    }
}

// ./processpool 5
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        return UsageError;
    }
    int sub_process_num = std::stoi(argv[1]);
    if (sub_process_num <= 0)
        return ArgError;

    srand((uint64_t)time(nullptr));

    // 1. 创建通信信道和子进程 -- bug
    ProcessPool *processpool_ptr = new ProcessPool(sub_process_num);
    processpool_ptr->CreateProcess(worker);
    processpool_ptr->Debug();

    // 2. 控制子进程
    CtrlProcessPool(processpool_ptr, 10);

    std::cout << "task run done" << std::endl;
    // sleep(100);

    // 3. 回收子进程: a:你怎么让所有的子进程退出
    processpool_ptr->KillAll();
    // b. 你怎么让所有已经退出的子进程把他进行资源回收wait/waitpid
    //  wait sub process;
    processpool_ptr->Wait();

    delete processpool_ptr;
    return 0;
}