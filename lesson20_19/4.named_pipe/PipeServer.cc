#include "Comm.hpp"
#include <unistd.h>

// 基于命名管道，创建一个进程池呢？？？
int main()
{
    Fifo fifo(Path);

    int rfd = open(Path, O_RDONLY);
    if (rfd < 0)
    {
        cerr << "open failed, errno: " << errno << ", errstring: " << strerror(errno) << endl;
        return 1;
    }
    // 如果我们的写端没打开，先读打开，open的时候就会阻塞，直到把写端打开，读open才会返回
    cout << "open success" << endl;

    char buffer[1024];
    while (true)
    {
        ssize_t n = read(rfd, buffer, sizeof(buffer) - 1);
        if (n > 0)
        {
            buffer[n] = 0;
            cout << "client say : " << buffer << endl;
        }
        else if (n == 0)
        {
            cout << "client quit, me too!!" << endl;
            break;
        }
        else
        {
            cerr << "read failed, errno: " << errno << ", errstring: " << strerror(errno) << endl;
            break;
        }
    }

    close(rfd);
    return 0;
}