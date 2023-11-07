#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdint>

static void TestSs();

int main()
{
    std::stringstream ss;
    uint8_t a = 1;
    uint8_t b = 2;

    ss << static_cast<int>(a) << ", " << b;
    std::cout << ss.str() << std::endl;
    return 0;
}

static void TestSs()
{
    uint32_t m_s1_1 = 0x12;
    uint32_t m_s1_2 = 0x34;
    std::stringstream ss;
    ss
            << "dec " << std::dec << m_s1_1
            << "hex " << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << m_s1_2
            // << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << (uint32_t)m_s1_1 & 0xffff
            ;
    std::cout << ss.str() << std::endl;
}