#include "io.h"
#include "vector"

class TestClass
{
public:
    std::string to_string()
    {
        return std::string(256, 'a');
    }

    friend std::ostream& operator <<(std::ostream& os, const TestClass& t)
    {
        return os << std::string(256, 'a');
    }
};

int main()
{
    std::ostringstream ss;
    std::vector<TestClass> tests(1000000);
    for (int i = 0; i < tests.size(); i++)
    {
        // 测试类内to_string()
        // ss << tests[i].to_string();

        // 测试利用重载<<实现的to_string()
        ss << SNet::IO::to_string(tests[i]);
    }
    return 0;
}