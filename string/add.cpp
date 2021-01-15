#include <iostream>

using namespace std;
int main()
{
    std::string name("Peter");
    std::string greeting;
    int count = 1;

    std::cout << sizeof(name) << std::endl;
    std::cout << name.size() << std::endl;
    std::cout << name << std::endl;
    greeting = "Hi " + name + to_string(count);
    std::cout << greeting << std::endl;
    std::cout << sizeof(greeting) << std::endl;
    std::cout << greeting.size() << std::endl;

    return 0;
}