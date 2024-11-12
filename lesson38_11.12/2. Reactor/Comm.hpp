#pragma once

#include <iostream>
#include <unistd.h>
#include <fcntl.h>

enum
{
    SOCKET_ERROR = 1,
    BIND_ERROR,
    LISTEN_ERROR,
    USAGE_ERROR,
    EPOLL_CREATE_ERROR,
};

void SetNonBlock(int fd)
{
    int fl = ::fcntl(fd, F_GETFL);
    if(fl < 0) 
    {
        return;
    }
    fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}