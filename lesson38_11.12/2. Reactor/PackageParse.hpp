#pragma once

#include <iostream>
#include "Connection.hpp"
#include "Protocol.hpp"
#include "CalCulate.hpp"

using namespace protocol_ns;

class PackageParse
{
public:
    static void Parse(Connection *conn)
    {
        // std::cout << "inbuffer: " << conn->Inbuffer() << std::endl;
        // 2. 分析数据，确认完整报文
        std::string package;
        Request req;
        Calculate cal;
        while (true)
        {
            std::cout << conn->Inbuffer() << std::endl;
            conn->AppendOutBuffer(conn->Inbuffer());
            break;
            // package = Decode(conn->Inbuffer());
            // if (package.empty())
            //     break;
            // std::cout << "------------------------begin---------------" << std::endl;
            // std::cout << "resq string:\n"
            //           << package << std::endl;
            // // 我敢保证，你一定读到了一个完整的json串.
            // // 3.反序列化
            // req.Deserialize(package);
            // // 4. 业务处理
            // Response resp = cal.Excute(req);

            // // 5. 对应答做序列化
            // std::string send_str;
            // resp.Serialize(&send_str);

            // std::cout << "resp Serialize:" << std::endl;
            // std::cout << send_str << std::endl;
            // // 6. 添加长度报头
            // send_str = Encode(send_str);
            // std::cout << "resp Encode:" << std::endl;
            // std::cout << send_str << std::endl;
            // conn->AppendOutBuffer(send_str);
        }

        if(!conn->OutbufferEmpty() && conn->_sender != nullptr)
        {
            conn->_sender(conn);

            // conn->_R->EnableReadWrite(conn->Sockfd(), true, true);
        }
    }
};