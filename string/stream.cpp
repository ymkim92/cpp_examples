#include <iostream>
#include <sstream>

int main()
{
    int a, b, c;
    char ch;
    std::string s;
    std::stringstream ss("10,20,30");

    ss >> s;
    std::cout << ss.str() << " : " << s << std::endl;
    ss.str("");
    ss.clear(); // **
    ss << s;
    ss >> a >> ch >> b >> ch >> c; 
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    return 0;
}