#include <iostream>
#include <memory>
#include "Dict.hpp"
#include "UdpServer.hpp"

using namespace dict_ns;

void Usage(std::string proc)
{
    std::cout << "Usage:\n\t" << proc << " local_port\n" << std::endl;
}

// ./udpserver port
// 云服务器的port默认都是禁止访问的。云服务器放开端口8080 ~ 8085
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERROR);
    }
    EnableScreen();
    // std::string ip = argv[1]; // TODO
    // 定义翻译模块
    Dict dict;

    // 网络部分
    uint16_t port = std::stoi(argv[1]);
    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(port,\
        std::bind(&Dict::Translate, &dict, std::placeholders::_1, std::placeholders::_2)); // C++14
    usvr->InitServer();
    usvr->Start();

    return 0;
}