#include <etl/crc16_modbus.h>
#include <stdio.h>

uint16_t GetCrcModbus(uint8_t* buf, size_t bufLen)
{
    uint16_t crc = etl::crc16_modbus(buf, buf+bufLen);
    return crc;
}
