#include <iostream>
#include <unistd.h>

int main()
{
    std::cout << "Pid is: " << getpid() << std::endl;
    sleep(1);

    // 0: 要更改工作目录
    // 0: 输入输出要进行重定向
    // daemon(1, 1); // fork father exit
    // 执行下面的代码的不是当前进程，而是当前进程的子进程
    while(true)
    {
        std::cout << "hello world" << std::endl;
        sleep(1);
    }

    return 0;
}



// #include <iostream>
// #include <string>
// #include <fstream>
// #include <jsoncpp/json/json.h>

// struct stu
// {
//     std::string name;
//     int age;
//     double weight;

// public:
//     void debug()
//     {
//         std::cout << name << std::endl;
//         std::cout << age << std::endl;
//         std::cout << weight << std::endl;
//     }
// };

// int main()
// {
//     std::ifstream in("out.txt");
//     if(!in.is_open()) return 1;
//     char buffer[1024];
//     in.read(buffer, sizeof(buffer));
//     in.close();

//     std::string json_string = buffer;
//     Json::Value root;
//     Json::Reader reader;
//     bool res = reader.parse(json_string, root);
//     (void)res;

//     struct stu zs;
//     zs.name = root["name"].asString();
//     zs.age = root["age"].asInt();
//     zs.weight =root["weight"].asDouble();

//     zs.debug();

//     // // 结构化数据
//     // struct stu zs = {"张三", 18, 70};

//     // // 转换成为字符串
//     // Json::Value root;
//     // root["name"] = zs.name;
//     // root["age"] = zs.age;
//     // root["weight"] = zs.weight;
//     // // root["self"] = root;

//     // // Json::FastWriter writer;
//     // Json::StyledWriter writer;
//     // std::string str = writer.write(root);

//     // std::ofstream out("out.txt");
//     // if(!out.is_open())
//     // {
//     //     std::cout << str;
//     //     return 1;
//     // }
//     // out << str;

//     // out.close();

//     return 0;
// }