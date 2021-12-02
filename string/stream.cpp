#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdint>

static void TestSs();

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

    TestSs();
    return 0;
}

static void TestSs()
{
    uint32_t m_s1_1 = 0x12;
    uint32_t m_s1_2 = 0x34;
    std::stringstream ss;
    ss
            << std::dec << m_s1_2
            << std::uppercase << std::setfill('0') << std::setw(1) << std::hex << (((uint32_t)m_s1_1 >> 16) & 0xf)
            // << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << (uint32_t)m_s1_1 & 0xffff
            ;
}