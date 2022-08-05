#include <array>
#include <iostream>

int main()
{
    std::array<int, 10> arr { 11, 12, 13, 14, 15 };
    std::cout << (arr.begin()+2).front() << std::endl;

    return 0;
}