#include <iostream>
#include <sstream>
#include <map>
#include <iomanip> // for setw

using namespace std;
static const std::map<const uint8_t, const std::string> SYSTEM_ERROR_MAP {
    { 0, "SYS_ERR_ALARM" },
    { 1, "SYS_ERR_UNUSED" },
    { 2, "SYS_ERR_UNUSED" },
    { 3, "SYS_ERR_SPI" },
    { 4, "SYS_ERR_OVERRANAGE" },
    { 5, "SYS_ERR_SELFTEST" },
    { 6, "SYS_ERR_FLASH" },
    { 7, "SYS_ERR_OVERRUN" },
    { 8, "SYS_ERR_NEWDAT_MAG" },
    { 9, "SYS_ERR_NEWDAT_BARO" },
    { 10, "SYS_ERR_ACCEL" },
    { 11, "SYS_ERR_MAG" },
    { 12, "SYS_ERR_GYRO" },
    { 13, "SYS_ERR_EKF" },
    { 14, "SYS_ERR_UNUSED" },
    { 15, "SYS_ERR_WATCHDOG" },
};

void PrintSystemError(uint16_t err)
{
    stringstream ss;
    ss << "ADIS16480 error: 0x" << setw(4) << setfill('0') << hex << static_cast<int>(err);
    for (uint8_t i = 0; i < SYSTEM_ERROR_MAP.size(); i++) {
        if (err & (1 << i)) {
            try {
                ss << " " << SYSTEM_ERROR_MAP.at(i) << "[" << static_cast<int>(i) << "]";
            } catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
            }
        }
    }
    std::cout << SYSTEM_ERROR_MAP.size() << std::endl;
    std::cout << ss.str() << std::endl;
}

int main()
{
    PrintSystemError(2);
    PrintSystemError(11);
    PrintSystemError(0x7000);
    return 0;
}
