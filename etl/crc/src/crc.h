#ifndef __CRC_H__
#define __CRC_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <string.h>

uint16_t GetCrcModbus(uint8_t* buf, size_t bufLen);

#ifdef __cplusplus
}
#endif
#endif // __CRC_H__