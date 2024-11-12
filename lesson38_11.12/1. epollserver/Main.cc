#include "EpollServer.hpp"
#include "Log.hpp"

#include <iostream>
#include <memory>

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
    std::unique_ptr<EpollServer> svr = std::make_unique<EpollServer>(port);
    svr->Loop();

    return 0;
}