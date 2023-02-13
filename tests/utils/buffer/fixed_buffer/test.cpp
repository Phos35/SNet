#include "fixed_buffer.h"
#include <iostream>

int main()
{
    FixedBuffer<32> fb;

    std::cout << "Input some string, then you can get the info of buffer\n";
    while (true)
    {
        std::string str;
        std::cin >> str;

        fb.write(str);

        std::cout << fb.to_string();
    }

    return 0;
}