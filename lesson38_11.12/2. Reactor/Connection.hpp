#pragma once

#include <iostream>
#include <string>
#include <functional>
#include "InetAddr.hpp"

class Connection;
class Reactor;
using func_t = std::function<void(Connection *)>;

// 一个fd，在我的心目当中，都被当做一个Conection
// listen sockfd 也是一样
class Connection
{
public:
    Connection(int sock) : _sock(sock), _R(nullptr)
    {
    }
    int Sockfd()
    {
        return _sock;
    }
    void SetEvents(int events)
    {
        _events = events;
    }
    uint32_t Events()
    {
        return _events;
    }
    void Register(func_t recver, func_t sender, func_t excepter)
    {
        _recver = recver;
        _sender = sender;
        _excepter = excepter;
    }
    void SetSelf(Reactor *R)
    {
        _R = R;
    }
    void AppendInBuffer(const std::string &buff)
    {
        _inbuffer += buff;
    }
    void AppendOutBuffer(const std::string &buff)
    {
        _outbuffer += buff;
    }
    std::string &Inbuffer() // 这里引用返回
    {
        return _inbuffer;
    }
    std::string &Outbuffer()
    {
        return _outbuffer;
    }
    bool OutbufferEmpty()
    {
        return _outbuffer.empty();
    }
    void OutbufferRemove(int n)
    {
        _outbuffer.erase(0, n);
    }
    void Close()
    {
        if(_sock>=0)
            ::close(_sock);
    }
    // void IsEnableWrite()
    // {

    // }
    ~Connection()
    {
    }

private:
    int _sock;
    std::string _inbuffer;
    std::string _outbuffer;

    InetAddr _addr;

    uint32_t _events; // Connection对象中，_sock关心的事件集合

public:
    func_t _recver;
    func_t _sender;
    func_t _excepter;
    Reactor *_R;

    // std::queue<std::string> results;
};