#include "etl_wrapper.h"

#include <etl/crc16_modbus.h>

uint16_t ETL_GetCrc16Modbus(uint8_t* buf, size_t bufLen)
{
    uint16_t crc = etl::crc16_modbus(buf, buf+bufLen);
    return crc;
}