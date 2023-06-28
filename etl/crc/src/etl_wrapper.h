#ifndef __ETL_WRAPPER_H__
#define __ETL_WRAPPER_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <string.h>

uint16_t ETL_GetCrc16Modbus(uint8_t* buf, size_t bufLen);

#ifdef __cplusplus
}
#endif



#endif // __ETL_WRAPPER_H__