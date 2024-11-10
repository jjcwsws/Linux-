#pragma once

#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include "Log.hpp"
#include "Comm.hpp"

static const int gsize = 128;

class Epoller
{
private:
    bool EventMethodCore(int fd, uint32_t events, int type)
    {
        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = fd;
        int n = ::epoll_ctl(_epfd, type, fd, &ev);
        if (n < 0)
        {
            LOG(ERROR, "epoll_ctl error!\n");
            return false;
        }
        LOG(INFO, "epoll_ctl add %d success!\n", fd); // TODO
        return true;
    }

public:
    Epoller()
    {
        _epfd = ::epoll_create(gsize);
        if (_epfd < 0)
        {
            LOG(FATAL, "epoll create error!\n");
            exit(EPOLL_CREATE_ERROR);
        }
        LOG(FATAL, "epoll create success, epfd: %d\n", _epfd);
    }
    bool AddEvent(int fd, uint32_t events)
    {
        return EventMethodCore(fd, events, EPOLL_CTL_ADD);
    }
    bool ModEvent(int fd, uint32_t events)
    {
        return EventMethodCore(fd, events, EPOLL_CTL_MOD);
    }
    bool DelEvent(int fd)
    {
        return ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, nullptr);
    }
    int Wait(struct epoll_event revs[], int num, int timeout)
    {
        int n = ::epoll_wait(_epfd, revs, num, timeout);
        return n;
    }
    ~Epoller()
    {
        if (_epfd >= 0)
            ::close(_epfd);
    }

private:
    int _epfd;
};