#include <iostream>
#include <functional>
#include <memory>
#include "TcpServer.hpp"
#include "Protocol.hpp"
#include "CalCulate.hpp"

using namespace protocol_ns;

void Usage(std::string proc)
{
    std::cout << "Usage:\n\t" << proc << " local_port\n"
              << std::endl;
}

using callback_t = std::function<Response(const Request &req)>;

class Service
{
public:
    Service(callback_t cb) : _cb(cb)
    {
    }
    void ServiceHelper(socket_sptr sockptr, InetAddr client)
    {
        int sockfd = sockptr->SockFd();
        LOG(DEBUG, "get a new link, info %s:%d, fd : %d\n", client.Ip().c_str(), client.Port(), sockfd);
        std::string clientaddr = "[" + client.Ip() + ":" + std::to_string(client.Port()) + "] ";
        std::string inbuffer;

        while (true)
        {
            sleep(5);

            Request req;
            // 你怎么保证一个大的字符串里面有没有完整的请求
            // 1.
            int n = sockptr->Recv(&inbuffer); // 你怎么保证你读到的是一个完整的request. 有效载荷的长度
            if (n < 0)
            {
                LOG(DEBUG, "client %s quit\n", clientaddr.c_str());
                break;
            }
           
            // 2. 分析数据，确认完整报文
            std::string package;
            while (true)
            {
                sleep(1);
                std::cout << "inbuffer: " << inbuffer << std::endl;
                package = Decode(inbuffer);
                if (package.empty())
                    break;
                std::cout << "------------------------begin---------------" << std::endl;
                std::cout << "resq string:\n"
                          << package << std::endl;
                // 我敢保证，你一定读到了一个完整的json串.
                // 3.反序列化
                req.Deserialize(package);
                // 4. 业务处理
                Response resp = _cb(req);

                // 5. 对应答做序列化
                std::string send_str;
                resp.Serialize(&send_str);

                std::cout << "resp Serialize:" << std::endl;
                std::cout << send_str << std::endl;
                // 6. 添加长度报头
                send_str = Encode(send_str);
                std::cout << "resp Encode:" << std::endl;
                std::cout << send_str << std::endl;

                // "len"\r\n"{             }"\r\n"
                sockptr->Send(send_str); // 本次不对发送做处理， EPOLL
            }
        }
    }

private:
    callback_t _cb;
};

// ./tcpserver port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        return 1;
    }
    uint16_t port = std::stoi(argv[1]);

    daemon(0, 0);
    
    EnableFile();
    Calculate cal;
    Service calservice(std::bind(&Calculate::Excute, &cal, std::placeholders::_1));
    io_service_t service = std::bind(&Service::ServiceHelper, &calservice, std::placeholders::_1, std::placeholders::_2);
    std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(port, service);

    tsvr->Loop();
    return 0;
}
