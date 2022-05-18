#include <iostream>
#include <string>

int main()
{
    std::string m_lrf_device_name {"LRF"};
    char name[3];
    name[0] = 'L';
    name[1] = 'R';
    name[2] = 'F';
    std::cout << !m_lrf_device_name.compare(0, 3, name) << std::endl;

    return 0;
}