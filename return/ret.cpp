#include <iostream>


bool equal(const int& a, const int& b)
{
    std::cout << a << ", " << b << std::endl;
    return a == b;
}

int main()
{
    bool ret = equal(1, 1);
    ret = ret && equal(2,2);
    ret = ret && equal(2,3);
    ret = ret && equal(2,3);
    return 0;
}