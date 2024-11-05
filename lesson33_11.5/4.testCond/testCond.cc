#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <unistd.h>

pthread_cond_t gcond = PTHREAD_COND_INITIALIZER; // 条件变量
pthread_mutex_t gmutex = PTHREAD_MUTEX_INITIALIZER; // 互斥锁

void *SlaverCore(void *args)
{
    std::string name = static_cast<const char *>(args);
    while (true)
    {
        // 1. 加锁
        pthread_mutex_lock(&gmutex);
        // 2. 一般条件变量是在加锁和解锁之间使用的
        pthread_cond_wait(&gcond, &gmutex); // gmutex:这个是，是用来被释放的[前一半]
        std::cout << "当前被叫醒的线程是: " << name << std::endl;
        // 3. 解锁
        pthread_mutex_unlock(&gmutex);
    }
}

void *MasterCore(void *args)
{
    sleep(3);
    std::cout << "master 开始工作..." << std::endl;
    std::string name = static_cast<const char *>(args);
    while (true)
    {
        // pthread_cond_signal(&gcond);// 唤醒其中一个队列首部的线程
        pthread_cond_broadcast(&gcond);// 唤醒队列中所有的线程
        std::cout << "master 唤醒一个线程..." << std::endl;
        sleep(1);
    }
}

void StartMaster(std::vector<pthread_t> *tidsptr)
{
    pthread_t tid;
    int n = pthread_create(&tid, nullptr, MasterCore, (void *)"Master Thread");
    if (n == 0)
    {
        std::cout << "create master success" << std::endl;
    }
    tidsptr->emplace_back(tid);
}

void StartSlaver(std::vector<pthread_t> *tidsptr, int threadnum = 3)
{
    for (int i = 0; i < threadnum; i++)
    {
        char *name = new char[64];
        snprintf(name, 64, "slaver-%d", i + 1); // thread-1
        pthread_t tid;
        int n = pthread_create(&tid, nullptr, SlaverCore, name);
        if (n == 0)
        {
            std::cout << "create success: " << name << std::endl;
            tidsptr->emplace_back(tid);
        }
    }
}

void WaitThread(std::vector<pthread_t> &tids)
{
    for (auto &tid : tids)
    {
        pthread_join(tid, nullptr);
    }
}

int main()
{
    std::vector<pthread_t> tids;
    StartMaster(&tids);
    StartSlaver(&tids, 5);
    WaitThread(tids);
    return 0;
}