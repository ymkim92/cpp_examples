#include <string>
#include <iostream>
#include <iomanip>

// checksum algorithm used in the TCP standard (RFC 1145)
//
uint16_t GetCheckSumRfc1145(uint8_t *buf, uint16_t size)
{
    uint8_t checksum[2] = {0,0};
    uint16_t i;
    for (i=0; i<size; i++)
    {
        checksum[0] += buf[i];
        checksum[1] += checksum[0];
    }

    return *((uint16_t *)checksum);
}

void PrintHex(uint8_t *data, uint16_t size)
{
    for (uint16_t i=0; i<size; i++)
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)data[i]<< '.';

    std::cout << '\n';
}

// B5 62 06 08 06 00 64 00 01 00 01 00 7A 12
// bool SetUbxCfg(uint8_t ubxClass, uint8_t ubxId, std::string &data)
void BuildUbxMessage(std::array<uint8_t, 256>& ret, const uint8_t& ubx_class, const uint8_t& ubx_id, const uint8_t* data, const uint16_t size)
{
    ret[0] = 0xb5;
    ret[1] = 0x62;
    ret[2] = ubx_class;
    ret[3] = ubx_id;
    ret[4] = size & 0xff;
    ret[5] = (size>>8) & 0xff;
    for (uint16_t i=0; i<size; i++) {
        ret[6+i] = data[i];
    }
    uint16_t crc = GetCheckSumRfc1145(&ret[2], size+4);
    
    ret[size+6] = crc & 0xff;
    ret[size+7] = (crc >> 8) & 0xff;
    printf("size %d\n", size);
}

int main()
{
    std::array<uint8_t, 256> message;
    uint8_t data[] = {0x64, 0, 1, 0, 1, 0};
    BuildUbxMessage(message, 6, 8, data, sizeof(data));
    std::cout << ".";
    PrintHex(message.data(), sizeof(data)+8);
    std::cout << "." << std::endl;
    return 0;
}