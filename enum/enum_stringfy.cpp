#include <iostream>
#include <sstream>

#define stringify( name )   #name 

enum class Adis16480SysErr {
    ALARM = 0,
    SPI = 3,
    OVERRANGE,
    SELFTEST,
    FLASH,
    SIZE = 16
};

int main()
{
    std::cout << stringify(Adis16480SysErr::FLASH) << std::endl;
    
    stringstream ss;
    ss << "ADIS16480 error: 0x" << setw(4) << setfill('0') << hex << static_cast<int>(err);
    for (uint8_t i = 0; i < Adis16480SysErr::SPI; i++) {
        if (err & (1 << i)) {
            ss << " " << SYSTEM_ERROR_MAP.at(i) << "[" << static_cast<int>(i) << "]";
        }
    }

    return 0;
}