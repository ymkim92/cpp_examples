#include <array>
#include <iostream>

int main()
{
    std::array<int, 10> arr { 11, 12, 13, 14, 15 };
    std::cout << arr[0] << std::endl;
    arr.at(0) = arr.at(1);
    std::cout << arr.at(0) << std::endl;
    std::fill(std::begin(arr), std::end(arr), 0);
    std::cout << arr[0] << std::endl;

    std::cout << arr[10] << std::endl;
    // std::cout << arr.at(10) << std::endl;

    return 0;
}