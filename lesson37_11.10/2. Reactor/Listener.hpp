#pragma once

#include <iostream>
#include <memory>
#include "Connection.hpp"
#include "Epoller.hpp"
#include "Socket.hpp"
#include "InetAddr.hpp"
#include "HandlerConnection.hpp"

using namespace socket_ns;

class Listener
{
public:
    Listener(int port, HandlerConnection &hc)
        : _port(port),
          _listensock(std::make_unique<TcpSocket>()),
          _hc(hc)
    {
        InetAddr addr("0", _port);
        _listensock->BuildListenSocket(addr);
    }
    void Accepter(Connection *conn) // conn一定是我们就绪的listensock
    {
        while (true)
        {
            InetAddr clientaddr;
            int code = 0;
            int sockfd = _listensock->Accepter(&clientaddr, &code);
            if (sockfd >= 0)
            {
                // TODO
                conn->_R->AddConnection(
                    sockfd,
                    EPOLLIN | EPOLLET,
                    std::bind(&HandlerConnection::HanlderRecv, &_hc, std::placeholders::_1),
                    std::bind(&HandlerConnection::HanlderSend, &_hc, std::placeholders::_1),
                    std::bind(&HandlerConnection::HanlderExcpet, &_hc, std::placeholders::_1));
            }
            else
            {
                if (code == EWOULDBLOCK || code == EAGAIN)
                {
                    LOG(DEBUG, "accepter all link!\n");
                    break;
                }
                else if (code == EINTR)
                {
                    LOG(DEBUG, "accepter interupt by signal!\n");
                    continue;
                }
                else
                {
                    LOG(WARNING, "accept error!\n");
                    break;
                }
            }
        }
    }
    int Sockfd()
    {
        return _listensock->SockFd();
    }
    ~Listener()
    {
        _listensock->Close();
    }

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensock;
    HandlerConnection &_hc;
};