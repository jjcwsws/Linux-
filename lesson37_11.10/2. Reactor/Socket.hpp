#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <pthread.h>
#include <sys/types.h>
#include <memory>
#include "Comm.hpp"
#include "InetAddr.hpp"
#include "Log.hpp"

// 模版方法模式
namespace socket_ns
{
    class Socket;
    const static int gbacklog = 8;
    using socket_sptr = std::shared_ptr<Socket>;

    // std::unique_ptr<Socket> listensock = std::make_unique<TcpSocket>();
    // listensock->BuildListenSocket();

    // std::unique_ptr<Socket> clientsock = std::make_unique<TcpSocket>();
    // clientsock->BuildClientSocket();

    // clientsock->send();
    // clientsock->Recv();

    class Socket
    {
    public:
        virtual void CreateSocketOrDie() = 0;
        virtual void BindSocketOrDie(InetAddr &addr) = 0;
        virtual void ListenSocketOrDie() = 0;
        virtual int Accepter(InetAddr *addr, int *errcode) = 0;
        virtual bool Connetcor(InetAddr &addr) = 0;
        virtual void SetSocketAddrReuse() = 0;
        virtual int SockFd() = 0;
        virtual int Recv(std::string *out) = 0;
        virtual int Send(const std::string &in) = 0;
        virtual void Close() = 0;

        // virtual void Recv() = 0;
        // virtual void Send() = 0;
        // virtual void other() = 0;

    public:
        void BuildListenSocket(InetAddr &addr)
        {
            CreateSocketOrDie();
            SetSocketAddrReuse();
            BindSocketOrDie(addr);
            ListenSocketOrDie();
        }
        bool BuildClientSocket(InetAddr &addr)
        {
            CreateSocketOrDie();
            return Connetcor(addr);
        }
        // void BuildUdpSocket()
        // {
        //     CreateSocketOrDie();
        //     BindSocketOrDie();
        // }
    };

    class TcpSocket : public Socket
    {
    public:
        TcpSocket(int fd = -1) : _sockfd(fd)
        {
        }
        void CreateSocketOrDie() override
        {
            // 1. 创建流式套接字
            _sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
            if (_sockfd < 0)
            {
                LOG(FATAL, "socket error");
                exit(SOCKET_ERROR);
            }
            SetNonBlock(_sockfd);
            LOG(DEBUG, "socket create success, sockfd is : %d\n", _sockfd);
        }
        void BindSocketOrDie(InetAddr &addr) override
        {
            // 2. bind
            struct sockaddr_in local;
            memset(&local, 0, sizeof(local));
            local.sin_family = AF_INET;
            local.sin_port = htons(addr.Port());
            local.sin_addr.s_addr = inet_addr(addr.Ip().c_str());

            int n = ::bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
            if (n < 0)
            {
                LOG(FATAL, "bind error\n");
                exit(BIND_ERROR);
            }
            LOG(DEBUG, "bind success, sockfd is : %d\n", _sockfd);
        }
        void ListenSocketOrDie() override
        {
            int n = ::listen(_sockfd, gbacklog);
            if (n < 0)
            {
                LOG(FATAL, "listen error\n");
                exit(LISTEN_ERROR);
            }
            LOG(DEBUG, "listen success, sockfd is : %d\n", _sockfd);
        }
        int Accepter(InetAddr *addr, int *errcode) override
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int sockfd = ::accept(_sockfd, (struct sockaddr *)&peer, &len);
            *errcode = errno;
            if (sockfd < 0)
            {
                LOG(WARNING, "accept error\n");
                return -1;
            }
            *addr = peer;
            // socket_sptr sock = std::make_shared<TcpSocket>(sockfd);
            SetNonBlock(sockfd);
            return sockfd;
        }
        bool Connetcor(InetAddr &addr)
        {
            // tcp client 要bind，不要显示的bind.
            struct sockaddr_in server;
            // 构建目标主机的socket信息
            memset(&server, 0, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_port = htons(addr.Port());
            server.sin_addr.s_addr = inet_addr(addr.Ip().c_str());

            int n = connect(_sockfd, (struct sockaddr *)&server, sizeof(server));
            if (n < 0)
            {
                std::cerr << "connect error" << std::endl;
                return false;
            }
            return true;
        }
        void SetSocketAddrReuse() override
        {
            int opt = 1;
            ::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        }
        int Recv(std::string *out) override
        {
            char inbuffer[4096];
            ssize_t n = ::recv(_sockfd, inbuffer, sizeof(inbuffer) - 1, 0);
            if (n > 0)
            {
                inbuffer[n] = 0;
                *out = inbuffer; // ??? +=
            }
            return n;
        }
        int Send(const std::string &in) override
        {
            int n = ::send(_sockfd, in.c_str(), in.size(), 0);
            return n;
        }
        int SockFd() override
        {
            return _sockfd;
        }
        void Close() override
        {
            if (_sockfd > -1)
                ::close(_sockfd);
        }

    private:
        int _sockfd;
    };

    // class SocketFactor
    // {
    // public:
    //     void Build
    // }
} // namespace socket_ns
