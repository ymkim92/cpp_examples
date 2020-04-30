#include <string>
#include <iostream>

void PrintString(std::string &buf)
{
    std::cout << buf << ":" << buf.length() << "\n";
    std::cout << buf << ":" << buf.capacity() << "\n";
}

int main()
{
    std::string a(8, '0');
    std::string c;
    std::string b = std::string(a, 3, 4);
    PrintString(a);
    a[1] = 0;
    std::cout << a << ":" << a.length() << "\n";
    std::cout << c << " len of c:" << c.length() << "\n";
    std::cout << a.c_str() << ":" << a.length() << "\n";
    std::cout << b << '\n';
    return 0;
}