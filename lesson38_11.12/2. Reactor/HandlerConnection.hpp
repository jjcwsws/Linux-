#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <functional>
#include "Log.hpp"
#include "Connection.hpp"

class HandlerConnection
{
public:
    HandlerConnection(func_t func):_func(func)
    {}
    void HanlderRecv(Connection *conn)
    {
        errno = 0;
        LOG(DEBUG, "HanlderRecv fd : %d\n", conn->Sockfd());
        while (true)
        {
            char buffer[1024];
            ssize_t n = ::recv(conn->Sockfd(), buffer, sizeof(buffer) - 1, 0);
            if(n > 0)
            {
                buffer[n] = 0;
                conn->AppendInBuffer(buffer);
            }
            else
            {
                if(errno == EWOULDBLOCK || errno == EAGAIN)
                {
                    break;
                }
                else if(errno == EINTR)
                {
                    continue;
                }
                else
                {
                    conn->_excepter(conn);
                    return; // 一定要提前返回
                }
            }
        }
        _func(conn);
    }
    void HanlderSend(Connection *conn)
    {
        // errno
        errno = 0;
        while(true)
        {
            ssize_t n = ::send(conn->Sockfd(), conn->Outbuffer().c_str(), conn->Outbuffer().size(), 0);
            if(n > 0)
            {
                // n 实际发送了多少
                conn->OutbufferRemove(n);
                if(conn->OutbufferEmpty()) break;
            }
            else if(n == 0)
            {
                break;
            }
            else
            {
                if(errno == EWOULDBLOCK || errno == EAGAIN)
                {
                    break; //只有这里会跳出循环，向下执行
                }
                else if(errno == EINTR)
                {
                    continue;
                }
                else
                {
                    conn->_excepter(conn);
                    return;
                }
            }
        }

        // 一定遇到了，发送缓冲区被写满的情况。
        if(!conn->OutbufferEmpty())
        {
            conn->_R->EnableReadWrite(conn->Sockfd(), true, true); // 开启对读事件关心
        }
        else
        {
            conn->_R->EnableReadWrite(conn->Sockfd(), true, false);
        }
    }
    void HanlderExcpet(Connection *conn)
    {
        errno = 0;
        LOG(DEBUG, "client quit : %d\n",conn->Sockfd());
        conn->_R->RemoveConnection(conn->Sockfd());
    }
private:
    func_t _func;
};