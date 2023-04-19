#include <stdio.h>
#include <string.h>
#include "ringbuffer.h"
void taskENTER_CRITICAL() {};
void taskEXIT_CRITICAL() {};

#define BUFSIZE         8
ringBuf_t ringBuf;
uint8_t message[BUFSIZE];
uint8_t rxBuf[BUFSIZE];

static void PrintBufInfo(ringBuf_t *ringBuf);
static void PrintBuf(uint8_t *buf, uint16_t size);

int main()
{
    uint8_t data1[] = "123";
    uint8_t data2[] = "4567";
    uint8_t data3[] = "89012";
	uint16_t size;
	
	ringBuf.m_ringBuf = message;

    RBF_InitRingBuf(&ringBuf);
    RBF_PutData(&ringBuf, BUFSIZE, data1, strlen(data1));
    PrintBufInfo(&ringBuf);
    RBF_PutData(&ringBuf, BUFSIZE, data2, strlen(data2));
    PrintBufInfo(&ringBuf);
    RBF_PutData(&ringBuf, BUFSIZE, data3, strlen(data3));
    PrintBufInfo(&ringBuf);

	size = RBF_GetData(&ringBuf, BUFSIZE, rxBuf);
	PrintBuf(rxBuf, size);
	size = RBF_GetData(&ringBuf, BUFSIZE, rxBuf);
	PrintBuf(rxBuf, size);

    return 0;
}


static void PrintBufInfo(ringBuf_t *ringBuf)
{
	printf("rd pos     %u\n", ringBuf->m_readPos);
	printf("wr pos     %u\n", ringBuf->m_writePos);
	printf("valid size %u\n", ringBuf->m_validSize);
}

static void PrintBuf(uint8_t *buf, uint16_t size)
{
	int i;
	for (i=0; i<size; i++)	
	{
		printf("%c", buf[i]);
	}
	printf("||\n");
}

void RBF_InitRingBuf(ringBuf_t *ringBuf)
{
	ringBuf->m_readPos = 0;
	ringBuf->m_writePos = 0;
	ringBuf->m_validSize = 0;
}

// **size of buf should be the same or larger than ringBufSize**
uint16_t RBF_GetData(ringBuf_t *ringBuf, uint16_t ringBufSize, uint8_t *buf)
{
	uint16_t rpos;
	uint16_t size1, size2;
	uint16_t ret;

	taskENTER_CRITICAL();
	ret = ringBuf->m_validSize;
	rpos = ringBuf->m_readPos;
	if ((rpos+ringBuf->m_validSize) > ringBufSize)
	{
		size1 = ringBufSize - rpos;
		size2 = ringBuf->m_validSize - size1;
	}
	else
	{
		size1 = ringBuf->m_validSize;
		size2 = 0;
	}

	memcpy(buf, ringBuf->m_ringBuf+rpos, size1);
	ringBuf->m_validSize -= size1;
	ringBuf->m_readPos += size1;
	if (size2)
	{
		memcpy(buf+size1, ringBuf->m_ringBuf, size2);
		ringBuf->m_validSize -= size2;
		ringBuf->m_readPos = size2;
	}
	taskEXIT_CRITICAL();

	return ret;
}

bool RBF_PutData(ringBuf_t *ringBuf, uint16_t ringBufSize, uint8_t *buf, uint16_t size)
{
	int i;

	for (i=0; i<size; i++)
	{
		ringBuf->m_ringBuf[ringBuf->m_writePos] = buf[i];
		ringBuf->m_writePos += 1;
		ringBuf->m_writePos %= ringBufSize;
		ringBuf->m_validSize += 1;
		if (ringBuf->m_validSize > ringBufSize)
		{
			ringBuf->m_validSize = ringBufSize;
			ringBuf->m_readPos += 1;
			ringBuf->m_readPos %= ringBufSize;
		}
	}
	if (ringBuf->m_validSize + size > ringBufSize)
	{
		return false;	// overflow
	}

	return true;
}
