#include "crc.h"
#include "etl_wrapper.h"

uint16_t GetCrcModbus(uint8_t* buf, size_t bufLen)
{
    return ETL_GetCrc16Modbus(buf, bufLen);
}
