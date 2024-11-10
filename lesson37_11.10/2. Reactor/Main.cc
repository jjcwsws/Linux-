#include <iostream>
#include <memory>
#include "Reactor.hpp"
#include "HandlerConnection.hpp"
#include "Connection.hpp"
#include "Listener.hpp"
#include "PackageParse.hpp"
#include "Log.hpp"

// ./selectserver port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " port" << std::endl;
        return 0;
    }
    uint16_t port = std::stoi(argv[1]);
    EnableScreen();
    std::unique_ptr<Reactor> react = std::make_unique<Reactor>(); // 主服务
    HandlerConnection hc(PackageParse::Parse); // IO通信无关协议。回调上层业务处理

    Listener listener(port, hc); // 负责连接的模块
    // 注册进入
    react->AddConnection(
        listener.Sockfd(),
        EPOLLIN | EPOLLET,
        std::bind(&Listener::Accepter, &listener, std::placeholders::_1),
        nullptr,
        nullptr);
    react->Dispatcher();

    return 0;
}