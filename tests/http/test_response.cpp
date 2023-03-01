#include "http_response.h"
#include "logger.h"
#include <fstream>
#include <time.h>

HTTPResponse gen_response()
{
    seed(time(0));
    HTTPResponse response;

    int len = rand() % 100 + 1;
}

void thread_func()
{
    HTTPResponse *response = new HTTPResponse();
    std::unordered_map<std::string, std::function<HTTPResponse(void)>> m;
    m["ABCDGHJ"] = std::function<HTTPResponse(void)>(gen_response);
    int times = 0;
    while (times < 1000000)
    {
        response->reset();
        *response = std::move(m["ABCDGHJ"]());
        std::string data = response->data();
        // printf("%d times\n", times++);
    }
}

int main()
{
    std::thread th1 = std::thread(thread_func);
    std::thread th2 = std::thread(thread_func);
    th1.join();
    th2.join();
    return 0;
}