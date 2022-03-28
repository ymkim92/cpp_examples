#include <iostream>

using namespace std;

int main()
{
    std::string name("0123456789");

    std::cout << name << std::endl;
    name = "abc";
    std::cout << name << std::endl;

    std::cout << name[7] << std::endl;
    name.clear();
    std::cout << name << endl;

    return 0;
}
