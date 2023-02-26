#include <iostream>

template <typename T1, typename T2>
class Test
{
public:
    T1 x_;
    T2 y_;

    Test(T1 x, T2 y)
    :x_(x), y_(y)
    {}

    void print()
    {
        std::cout << "X: " << x_ << ", Y: " << y_;
    }
};

int main()
{
    Test<int, double> t(1, 5.2);
    t.print();
    return 0;
}