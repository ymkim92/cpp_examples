#include <iostream>
#include <map>

std::map<uint8_t, uint16_t> num_pair = {
    { 2, 100 },
    { 3, 1000 },
};

int main()
{
    try {
        std::cout << num_pair.at(2) << std::endl;
        std::cout << num_pair.at(3) << std::endl;
        std::cout << num_pair.at(4) << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cout << "OUt of Range error" << std::endl;
    }
}