#ifndef UTILS_RINGBUFFER_H_
#define UTILS_RINGBUFFER_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	uint8_t *m_ringBuf;
	uint16_t m_readPos;
	uint16_t m_writePos;
	uint16_t m_validSize;
} ringBuf_t;

void RBF_InitRingBuf(ringBuf_t *ringBuf);
uint16_t RBF_GetData(ringBuf_t *ringBuf, uint16_t ringBufSize, uint8_t *buf);
bool RBF_PutData(ringBuf_t *ringBuf, uint16_t ringBufSize, uint8_t *buf, uint16_t size);


#endif /* UTILS_RINGBUFFER_H_ */


