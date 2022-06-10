#include <iostream>
#include <cstring>

int main()
{
    float ff=1.;
    std::cout << ff << std::endl;
    uint32_t uu = 0xffffffff;
    uint32_t ii;
    ff = uu;
    std::cout << uu << std::endl;
    std::cout << ff << std::endl;
    
    std::cout << std::endl;
    uu = 0xfffff;
    memcpy(&ff, &uu, sizeof(uint32_t));
    std::cout << uu << std::endl;
    std::cout << ff << std::endl;
    memcpy(&ii, &ff, sizeof(uint32_t));
    std::cout << ii << std::endl;
    std::cout << std::endl;
    

    std::cout << *reinterpret_cast<float *>(&uu) << std::endl;
    std::cout << std::endl;

    uu = 0x7ffff;
    ff = uu;
    std::cout << uu << std::endl;
    std::cout << ff << std::endl;
    return 0;
}