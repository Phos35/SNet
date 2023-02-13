#include "byte_buffer.h"
#include <iostream>

int main()
{
    ByteBuffer bb(32);

    std::cout << "Input some string, then you can get the info of buffer\n";
    while (true)
    {
        std::string str;
        std::cin >> str;

        if(str.find("END") != std::string::npos)
            break;
        else if(str.find("RELEASE") != std::string::npos)
            bb.release(5);
        else if(str.find("READ") != std::string::npos)
            std::cout << bb.read(5);
        else if(str.find("COMPACT") != std::string::npos)
            {}// bb.compact(); 测试在该函数是public时进行，权限修改后取消
        else
            bb.write(str);

        std::cout << bb.to_string();
    }

    return 0;
}