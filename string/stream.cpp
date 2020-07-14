#include <iostream>
#include <sstream>

int main()
{
    int a, b, c;
    char ch;
    std::string s;
    std::stringstream ss("10,20,30");
    bool ret;

    ss >> s;
    std::cout << ss.str() << " : " << s << std::endl;
    ss.str("");
    ss.clear(); // **
    ss << s;
    ss >> a >> ch >> b >> ch >> c; 
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;

    ss.str("");
    ss.clear(); // **
    ss << s;
    ss >> a >> ch; 
    ret = ss.eof();
    std::cout << a << ", eof:" << ret << std::endl;
    ss >> b >> ch; 
    ret = ss.eof();
    std::cout << b << ", eof:" << ret << std::endl;
    ss >> c >> ch; 
    ret = ss.eof();
    std::cout << c << ", eof:" << ret << std::endl;
    return 0;
}