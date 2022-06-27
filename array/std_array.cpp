#include <array>
#include <iostream>

int main()
{
    std::array<int, 3> arr { 11, 12, 13 };
    std::cout << arr.begin() << std::endl;
    std::cout << arr.front() << std::endl;
    std::cout << arr.back() << std::endl;

    for (auto &a: arr) {
        std::cout << a << " ";
    }
    std::cout << std::endl;
    return 0;
}