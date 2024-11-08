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
#include <sys/wait.h>

#include "InetAddr.hpp"
#include "Log.hpp"

enum
{
    SOCKET_ERROR = 1,
    BIND_ERROR,
    LISTEN_ERROR,
    USAGE_ERROR
};

const static int defaultsockfd = -1;
const static int gbacklog = 16;

class TcpServer;

class ThreadData
{
public:
    ThreadData(int fd, InetAddr addr, TcpServer *s):sockfd(fd), clientaddr(addr), self(s)
    {}
public:
    int sockfd;
    InetAddr clientaddr;
    TcpServer *self;
};

using func_t = std::function<std::string (const std::string &)>;

class TcpServer
{
public:
    TcpServer(int port, func_t func)
    : _port(port), _listensock(defaultsockfd), _isrunning(false), _func(func)
    {
    }
    void InitServer()
    {
        // 1. 创建流式套接字
        _listensock = ::socket(AF_INET, SOCK_STREAM, 0);
        if (_listensock < 0)
        {
            LOG(FATAL, "socket error");
            exit(SOCKET_ERROR);
        }
        LOG(DEBUG, "socket create success, sockfd is : %d\n", _listensock);

        // 2. bind
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = INADDR_ANY;

        int n = ::bind(_listensock, (struct sockaddr *)&local, sizeof(local));
        if (n < 0)
        {
            LOG(FATAL, "bind error");
            exit(BIND_ERROR);
        }
        LOG(DEBUG, "bind success, sockfd is : %d\n", _listensock);

        // 3. tcp是面向连接的，所以通信之前，必须先建立连接。服务器是被连接的
        //    tcpserver 启动，未来首先要一直等待客户的连接到来
        n = ::listen(_listensock, gbacklog);
        if (n < 0)
        {
            LOG(FATAL, "listen error");
            exit(LISTEN_ERROR);
        }
        LOG(DEBUG, "listen success, sockfd is : %d\n", _listensock);
    }
    void Service(int sockfd, InetAddr client)
    {
        LOG(DEBUG, "get a new link, info %s:%d, fd : %d\n", client.Ip().c_str(), client.Port(), sockfd);

        std::string clientaddr = "[" + client.Ip() + ":" + std::to_string(client.Port()) + "]# ";
        while (true)
        {
            char inbuffer[1024];
            // 1. tcp 面向字节流，不能保证inbuffer，是一个完整的命令字符串，以"ls -a -l"
            // 2. "ls -a -l" -> ls -       a -l
            // 3. 所以， 对于tcp，我们必须保证在应用层面，保证我们收到的是一个完整的请求！
            ssize_t n = recv(sockfd, inbuffer, sizeof(inbuffer) - 1, 0); // bug
            if (n > 0)
            {
                inbuffer[n] = 0;
                std::cout << clientaddr << inbuffer << std::endl;

                std::string result = _func(inbuffer);

                send(sockfd, result.c_str(), result.size(), 0);
            }
            else if (n == 0)
            {
                // client 退出&&关闭连接了
                LOG(INFO, "%s quit\n", clientaddr.c_str());
                break;
            }
            else
            {
                LOG(ERROR, "read error\n", clientaddr.c_str());
                break;
            }
        }

        ::close(sockfd); // 文件描述符泄漏
    }
    static void *HandlerSock(void *args) // IO 和 业务进行解耦
    {
        pthread_detach(pthread_self());
        ThreadData *td = static_cast<ThreadData *>(args);
        td->self->Service(td->sockfd, td->clientaddr);
        delete td;
        return nullptr;
    }
    void Loop()
    {
        _isrunning = true;
        // 4. 不能直接接受数据，先获取连接
        while (_isrunning)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int sockfd = ::accept(_listensock, (struct sockaddr *)&peer, &len);
            if (sockfd < 0)
            {
                LOG(WARNING, "accept error\n");
                continue;
            }
            //采用多线程
            pthread_t t;
            ThreadData *td = new ThreadData(sockfd, InetAddr(peer), this);
            pthread_create(&t, nullptr, HandlerSock, td); //将线程分离
        }
        _isrunning = false;
    }
    ~TcpServer()
    {
        if (_listensock > defaultsockfd)
            ::close(_listensock);
    }

private:
    uint16_t _port;
    int _listensock;
    bool _isrunning;

    func_t _func;
};