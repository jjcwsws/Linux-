#include "Comm.hpp"
#include "Fifo.hpp"
#include <unistd.h>

int main()
{
    key_t key = GetShmKeyOrDie();
    std::cout << "key: " << ToHex(key) << std::endl;
    // sleep(2);

    int shmid = GetShm(key, defaultsize);
    std::cout << "shmid: " << shmid << std::endl;
    // sleep(2);

    char *addr = (char *)ShmAttach(shmid);
    std::cout << "Attach shm success, addr: " << ToHex((uint64_t)addr) << std::endl;
    // sleep(5);

    memset(addr, 0, defaultsize);
    Sync syn;
    syn.OpenWriteOrDie();

    // 可以进行通信了
    for (char c = 'A'; c <= 'Z'; c++) // pipe, fifo, ->read/write->系统调用， shm -> 没有使用系统调用！！
    {
        addr[c - 'A'] = c;
        sleep(1);
        syn.Wakeup();
    }

    ShmDetach(addr);
    std::cout << "Detach shm success, addr: " << ToHex((uint64_t)addr) << std::endl;
    sleep(5);

    return 0;
}