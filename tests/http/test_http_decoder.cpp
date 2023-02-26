#include "http_decoder.h"
#include <string>
#include <fstream>
#include <cassert>
#include <cstring>

void uncomplete_test()
{
    char buf[1024] = {0};
    std::ifstream input_file("test.txt");
    assert(input_file.is_open() == true);

    HTTPDecoder decoder;
    for (int i = 1; i < 500; i++)
    {
        input_file.readsome(buf, i);
        printf("start with: %s\n\n--------\n", buf);
        HTTPRequest::Ptr request = decoder.decode(buf);

        printf("Message decode: %s\n", request->get_result() == Message::DeocdeResult::FAILURE ? "FAILURE" : "SUCCESS");
        printf("%s\n", request->to_string().c_str());

        delete request;
        memset(buf, 0, 1024);
        input_file.clear();
        input_file.seekg(0, std::ios::beg);
    }
}

int main()
{
    char buf[1024] = {0};
    std::ifstream input_file("wrong_data.txt");
    assert(input_file.is_open() == true);

    HTTPDecoder decoder;
    input_file.readsome(buf, 1024);
    HTTPRequest::Ptr request = decoder.decode(buf);

    printf("Message decode: %s\n", request->get_result() == Message::DeocdeResult::FAILURE ? "FAILURE" : "SUCCESS");
    printf("%s\n", request->to_string().c_str());

    return 0;
}