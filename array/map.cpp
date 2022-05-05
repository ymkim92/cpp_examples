#include <iostream>
#include <map>

int main()
{
    std::map<int, int> m;

    m.insert(std::pair<int, int>(1, 2));
    m.insert(std::pair<int, int>(10, 5));

    std::cout << m[10] << std::endl;
    std::cout << m[2] << std::endl;

    std::cout << m.at(10) << std::endl;
    std::cout << m.at(3) << std::endl;
    return 0;
}