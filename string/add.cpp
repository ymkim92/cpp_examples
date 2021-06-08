#include <iostream>

using namespace std;
string add(string name);

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

    std::cout << add(name) << endl;
    std::cout << add(name) << endl;

    name.clear();
    std::cout << name << endl;

    return 0;
}

string add(string name)
{
    static string greet("Hi ");
    greet += name;
    return greet;
}