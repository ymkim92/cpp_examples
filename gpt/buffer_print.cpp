// print a std::array or std::vector of uint8_t elements using a C++ template

#include <array>
#include <vector>
#include <iostream>
#include <sstream>

template<typename Container>
std::string GetBufferOutput(const Container& buffer, size_t print_size)
{
    std::stringstream ss {};
    size_t size = std::min(buffer.size(), print_size);
    for (size_t i = 0; i < size; i++) {
        ss << static_cast<int>(buffer[i]) << " ";
    }
    return ss.str();
}

int main()
{
    std::array<uint8_t, 4> arr { 1, 2, 3, 4 };
    std::vector<uint8_t> vec { 5, 6, 7, 8 };

    std::cout << GetBufferOutput(arr, 3) << std::endl;
    std::cout << GetBufferOutput(vec, 5) << std::endl;
    return 0;
}