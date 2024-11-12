#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <sys/epoll.h>
#include "InetAddr.hpp"
#include "Socket.hpp"
#include "Log.hpp"

using namespace socket_ns;

class EpollServer
{
    const static int gnum = 64;

public:
    EpollServer(uint16_t port = 8888)
        : _port(port),
          _listensock(std::make_unique<TcpSocket>()),
          _epfd(-1)
    {
        // 1. 创建listensock
        InetAddr addr("0", _port);
        _listensock->BuildListenSocket(addr);

        // 2. 创建epoll模型
        _epfd = ::epoll_create(128);
        if (_epfd < 0)
        {
            LOG(FATAL, "epoll_create error\n");
            exit(5);
        }
        LOG(DEBUG, "epoll_create success, epfd: %d\n", _epfd);
        // 3. 只有一个listensock, listen sock 关心的事件：读事件
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = _listensock->SockFd(); //???
        epoll_ctl(_epfd, EPOLL_CTL_ADD, _listensock->SockFd(), &ev);
    }
    ~EpollServer()
    {
        _listensock->Close();
        if (_epfd >= 0)
            ::close(_epfd);
    }

    void handlerEvent(int num)
    {
        for (int i = 0; i < num; i++)
        {
            // 你想知道什么呢？
            uint32_t revents = _revs[i].events;
            int sockfd = _revs[i].data.fd;

            // 读事件就绪
            if (revents & EPOLLIN)
            {
                if (sockfd == _listensock->SockFd())
                {
                    InetAddr clientaddr;
                    int newfd = _listensock->Accepter(&clientaddr); // 会不会被阻塞？不会!
                    if (newfd < 0)
                        continue;

                    // 获取新链接成功
                    // 读取或者写入呢？不能！谁知道newfd上面有对应的事件就绪了呢？epoll
                    struct epoll_event ev;
                    ev.events = EPOLLIN;
                    ev.data.fd = newfd;
                    epoll_ctl(_epfd, EPOLL_CTL_ADD, newfd, &ev);
                    LOG(DEBUG, "_listensock ready, accept done, epoll_ctl done, newfd is: %d\n", newfd);
                }
                else
                {
                    char buffer[1024];
                    ssize_t n = ::recv(sockfd, buffer, sizeof(buffer), 0); // 会不会被阻塞？只能recv一次。不会
                    if (n > 0)
                    {
                        LOG(DEBUG, "normal fd %d ready, recv begin...\n", sockfd);
                        buffer[n] = 0;
                        std::cout << "client say# " << buffer << std::endl;

                        std::string echo_string = "server echo# ";
                        echo_string += buffer;
                        ::send(sockfd, echo_string.c_str(), echo_string.size(), 0);
                    }
                    else if (n == 0)
                    {
                        LOG(DEBUG, "normal fd %d close, me too!\n", sockfd);
                        // 对端连接关闭了
                        ::epoll_ctl(_epfd, EPOLL_CTL_DEL, sockfd, nullptr); // 要删除的fd，必须是合法的！
                        ::close(sockfd);
                    }
                    else
                    {
                        ::epoll_ctl(_epfd, EPOLL_CTL_DEL, sockfd, nullptr); // 要删除的fd，必须是合法的！
                        ::close(sockfd);
                    }
                }
            }
        }
    }

    void Loop()
    {
        int timeout = -1;
        while (true)
        {
            int n = ::epoll_wait(_epfd, _revs, gnum, timeout);
            switch (n)
            {
            case 0:
                LOG(DEBUG, "epoll_wait timeout...\n");
                break;
            case -1:
                LOG(DEBUG, "epoll_wait failed...\n");
                break;
            default:
                LOG(DEBUG, "epoll_wait haved event ready..., n : %d\n", n);
                // handlerEvent(n);
                break;
            }
        }
    }

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensock;
    int _epfd;

    struct epoll_event _revs[gnum];
};