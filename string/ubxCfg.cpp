#include <string>
#include <iostream>
#include <iomanip>

// checksum algorithm used in the TCP standard (RFC 1145)
//
uint16_t GetCheckSumRfc1145(std::string const&a_Buffer)
{
    uint8_t checksum[2] = {0,0};
    int i;
    for (i=0; i<a_Buffer.length(); i++)
    {
        checksum[0] += a_Buffer[i];
        checksum[1] += checksum[0];
    }

    return *((uint16_t *)checksum);
}

void PrintHex(std::string data)
{
    for (int i=0; i<data.length(); i++)
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)data.at(i)<< '.';

    std::cout << '\n';
}


bool SetUbxCfg(uint8_t ubxClass, uint8_t ubxId, std::string &data)
{
    std::string header(4, '0');
    std::string crc(2, '0');
    
    header.at(0) = 0xb5;
    header.at(1) = 0x62;
    header.at(2) = ubxClass;
    header.at(3) = ubxId;

    PrintHex(header.substr(2));

    uint16_t ui16Crc = GetCheckSumRfc1145(header.substr(2)+data);
    
    crc.at(0) = ui16Crc & 0xff;
    crc.at(1) = ui16Crc >> 8;
    PrintHex(crc);
    // input.at(6) = 0x21;
    // input.at(7) = 0x69;

}

int main()
{
    std::string data(2, '\0');
    SetUbxCfg(6, 0x1b, data);
    return 0;
}