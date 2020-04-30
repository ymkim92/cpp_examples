#include <iostream>
#include <sstream>

int main()
{
    std::stringstream poleId;
    poleId << 1;
    std::cout << poleId.str() << std::endl;
    return 0;
}