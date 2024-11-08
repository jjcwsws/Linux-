#include "TcpServer.hpp"
#include "CommandExcute.hpp"
#include <memory>

void Usage(std::string proc)
{
    std::cout << "Usage:\n\t" << proc << " local_port\n" << std::endl;
}

// ./tcpserver port
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        return 1;
    }
    EnableScreen();
    uint16_t port = std::stoi(argv[1]);
    Command cmd("./safe.txt");

    func_t cmdExec = std::bind(&Command::Excute, &cmd, std::placeholders::_1);
    std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(port, cmdExec);
    tsvr->InitServer();
    tsvr->Loop();
    return 0;
}