#include <etl/crc16_modbus.h>
#include <iostream>

int main()
{
    std::string data("123456789");

    uint16_t crc = etl::crc16_modbus_t16(data.begin(), data.end());

    std::cout << "CRC16 of this " << data << ":" << crc << std::endl;

    std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
    uint16_t crc1 = etl::crc16_modbus(data1.begin(), data1.end());
    std::cout << "CRC16 of this " << data1[0] << ":" << crc1 << std::endl;
    return 0;
}