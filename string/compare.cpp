#include <iostream>
#include <stdint.h>

using namespace std;

namespace UBX
{
    constexpr char message2[] = "\x61\x62\x63\x64";
}

int main()
{
    std::string message("abcd");

    std::cout << sizeof(message) << std::endl;
    std::cout << message.size() << std::endl;
    std::cout << sizeof(UBX::message2) << std::endl;

    cout << message[0] << endl;
    cout << UBX::message2[0] << endl;

    cout << "***********************************************" << endl;
    if (message[0] == 'a')
        std::cout << "a" << std::endl;
    if (message[0] == 0x61)
        std::cout << "0x61" << std::endl;

    cout << "***********************************************" << endl;
    if (UBX::message2[0] == 'a')
        std::cout << "a" << std::endl;
    if (UBX::message2[0] == 0x61)
        std::cout << "0x61" << std::endl;

    if (message.compare(UBX::message2) == 0)
        cout << "same" << endl;
        
    return 0;
}