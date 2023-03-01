#include <string>
#include <thread>
#include <vector>

std::string get_str()
{
    return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 36\r\n\r\n<html>\n    <h1> Hello! </h1>\n</html>";
}

void thread_fun()
{
    printf("start\n");
    while (true)
    {
        if(get_str() != "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 36\r\n\r\n<html>\n    <h1> Hello! </h1>\n</html>")
        {
            printf("Error Occurred\n");
        }
    }
    printf("end\n");
}

int main()
{
    std::vector<std::thread> threads_;
    for (int i = 0; i < 10; i++)
    {
        threads_.push_back(std::thread(thread_fun));
    }

    for (int i = 0; i < 10; i++)
    {
        threads_[i].join();
    }
    return 0;
}