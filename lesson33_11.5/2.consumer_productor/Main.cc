#include "BlockQueue.hpp"
#include "Thread.hpp"
#include "Task.hpp"
#include <string>
#include <vector>
#include <unistd.h>
#include <ctime>

using namespace ThreadModule;
int a = 10;
using blockqueue_t = BlockQueue<Task>;

void PrintHello()
{
    std::cout << "hello world" << std::endl;
}
// class ThreadData
// {
// private:
//     blockqueue_t &bq;
//     std::string who;
// };


void Consumer(blockqueue_t &bq)
{
    while (true)
    {
        // 1. 从blockqueue取下来任务
        Task t;
        bq.Pop(&t);
        // 2. 处理这个任务
        t(); //消费者私有
        // std::cout << "Consumer Consum data is : " << t.ResultToString() << std::endl;
    }
}

void Productor(blockqueue_t &bq)
{
    srand(time(nullptr)^pthread_self());
    int cnt = 10;
    while (true)
    {
        sleep(1);
        // // 1. 获取任务
        // int a = rand() % 10 + 1;
        // usleep(1234);
        // int b = rand() % 20 + 1;
        // Task t(a, b);
        // 2. 把获取的任务，放入blockqueue
        Task t = PrintHello;
        bq.Enqueue(t);
        // std::cout << "Productor product data is : " << t.DebugToString() << std::endl;
    }
}

void StartComm(std::vector<Thread<blockqueue_t>> *threads, int num, blockqueue_t &bq, func_t<blockqueue_t> func)
{
    for (int i = 0; i < num; i++)
    {
        std::string name = "thread-" + std::to_string(i + 1);
        threads->emplace_back(func, bq, name);
        threads->back().Start();
    }
}

void StartConsumer(std::vector<Thread<blockqueue_t>> *threads, int num, blockqueue_t &bq)
{
    StartComm(threads, num, bq, Consumer);
}

void StartProductor(std::vector<Thread<blockqueue_t>> *threads, int num, blockqueue_t &bq)
{
    StartComm(threads, num, bq, Productor);
}

void WaitAllThread(std::vector<Thread<blockqueue_t>> &threads)
{
    for (auto &thread : threads)
    {
        thread.Join();
    }
}

int main()
{
    blockqueue_t *bq = new blockqueue_t(5);
    std::vector<Thread<blockqueue_t>> threads;
    // std::vector<Thread<ThreadData>> threads;

    StartConsumer(&threads, 3, *bq);
    StartProductor(&threads, 1, *bq);
    WaitAllThread(threads);

    return 0;
}