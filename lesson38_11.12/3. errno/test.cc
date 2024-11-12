#include <iostream>
#include <cerrno>
#include <unistd.h>
#include <pthread.h>


void *threadrun1(void *args)
{
    while(true)
    {
        std::cout << "thread 1, errno: " << errno << std::endl;
        sleep(1);
    }
}

void *threadrun2(void *args)
{
    while(true)
    {
        errno++;
        std::cout << "thread2, errno: " << errno << std::endl;
        sleep(1);
    }
}


int main()
{
    pthread_t tid1, tid2;
    pthread_create(&tid1, nullptr, threadrun1, nullptr);
    pthread_create(&tid2, nullptr, threadrun2, nullptr);

    pthread_join(tid1, nullptr);
    pthread_join(tid2, nullptr);
}