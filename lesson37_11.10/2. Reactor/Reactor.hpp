#pragma once

#include <iostream>
#include <unordered_map>
#include "Epoller.hpp"
#include "Connection.hpp"

// TcpServer -> rename
// TcpServer: 对于已经就绪的事件，进行通知！
// 这个tcpserver提供_connections的管理工作
// TcpServer就是Reactor(反应堆)
class Reactor // 事件派发器
{
    const static int gnum = 64;

public:
    Reactor() : _isrunning(false)
    {
    }
    // 添加listensock到TcpServer， TcpServer自己要独立维护Connection
    void AddConnection(int fd, uint32_t events, func_t recver, func_t sender, func_t excepter)
    {
        // 1. 构建Connection
        Connection *conn = new Connection(fd);
        conn->SetEvents(events);
        conn->Register(recver, sender, excepter);
        conn->SetSelf(this);

        // 2. 向内核中设定对fd的关心
        _epller.AddEvent(conn->Sockfd(), conn->Events());

        // 3. 向_connections添加connection对象
        _connections.insert(std::make_pair(conn->Sockfd(), conn));
    }
    void EnableReadWrite(int sockfd, bool readable, bool writeable)
    {
        uint32_t events = (readable?EPOLLIN:0) | (writeable ? EPOLLOUT : 0) | EPOLLET;
        if(ConnectionIsExists(sockfd))
        {
            // 1. 修改我们写的connection关心的事件
            _connections[sockfd]->SetEvents(events);

            // 2. 写透到内核中
            _epller.ModEvent(sockfd, events);
        }
    }
    void RemoveConnection(int sockfd)
    {
        if(!ConnectionIsExists(sockfd)) return;
        // 1. 去掉epoll对该文件fd的关心
        _epller.DelEvent(sockfd);
        // 2. 服务器端也要关闭sockfd
        _connections[sockfd]->Close();
        // 3. 释放connection
        delete _connections[sockfd];
        _connections.erase(sockfd);
    }
    bool ConnectionIsExists(int sockfd)
    {
        auto iter = _connections.find(sockfd);
        return iter != _connections.end();
    }
    void LoopOnce(int timeout)
    {
        int n = _epller.Wait(revs, gnum, timeout);
        for (int i = 0; i < n; i++)
        {
            int sockfd = revs[i].data.fd;
            uint32_t revents = revs[i].events;

            if (revents & EPOLLHUP)
                revents |= (EPOLLIN | EPOLLOUT);
            if (revents & EPOLLERR)
                revents |= (EPOLLIN | EPOLLOUT);
            if (revents & EPOLLIN)
            {
                if (ConnectionIsExists(sockfd) && (_connections[sockfd]->_recver != nullptr))
                {
                    _connections[sockfd]->_recver(_connections[sockfd]);
                }
            }
            if (revents & EPOLLOUT)
            {
                if (ConnectionIsExists(sockfd) && (_connections[sockfd]->_sender != nullptr))
                {
                    _connections[sockfd]->_sender(_connections[sockfd]);
                }
            }
        }
    }
    // 事件派发核心函数
    void Dispatcher()
    {
        _isrunning = true;
        int timeout = 3000;
        while (_isrunning)
        {
            LoopOnce(timeout);
            // 处理其他事情
            Debug();
        }
        _isrunning = false;
    }
    void Debug()
    {
        std::cout << "------------------------------------" << std::endl;
        for(auto &connection : _connections)
        {
            std::cout << "fd : " << connection.second->Sockfd() << ", ";
            uint32_t events = connection.second->Events();
            if((events & EPOLLIN) && (events & EPOLLET))
                std::cout << "EPOLLIN | EPOLLET, ";
            if((events & EPOLLOUT) && (events & EPOLLET))
                std::cout << "EPOLLOUT | EPOLLET";
            std::cout << std::endl;
        }
        std::cout << "------------------------------------" << std::endl;
    }
    ~Reactor() {}

private:
    std::unordered_map<int, Connection *> _connections; // int : sockfd
    struct epoll_event revs[gnum];
    Epoller _epller;
    bool _isrunning;
};