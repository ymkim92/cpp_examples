#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
 

#define	LRF_DEVICE_NAME_LEN		3

void SPI_PackLrfProbeStatus(uint32_t *dst, bool probeFlag, uint8_t *deviceName3Chars);
void SPI_UnpackLrfProbeStatus(bool *probeFlag, uint8_t *deviceName3Chars, uint32_t src);
void SPI_PackLrfConfig(uint32_t *dst, bool laserPointer, uint8_t cmFreq, uint8_t minRange, uint8_t maxRange);
void SPI_UnpackLrfConfig(bool *laserPointer, uint8_t *cmFreq, uint8_t *minRange, uint8_t *maxRange, uint32_t src);
void SPI_PackLrfConfigStatus(uint32_t *dst, bool minRangeFlag, bool maxRangeFlag, bool cmFlag);
void SPI_UnpackLrfConfigStatus(bool *minRangeFlag, bool *maxRangeFlag, bool *cmFlag, uint32_t src);

int main()
{
	uint32_t data;
	{
		int i;
		bool probeFlag;
		uint8_t name[3];

		SPI_PackLrfProbeStatus(&data, true, "abc");
		printf("0x%X\n", data);
		SPI_UnpackLrfProbeStatus(&probeFlag, name, 0xffffffff);
		printf("flag: %d, name: ", probeFlag);
		for (i=0; i<3; i++)
			printf("%c", name[i]);

		printf("\n");

		SPI_UnpackLrfProbeStatus(&probeFlag, name, data);
		printf("flag: %d, name: ", probeFlag);
		for (i=0; i<3; i++)
			printf("%c", name[i]);

		printf("\n");
	}
	{
		bool laserPointer;
		uint8_t cmFreq;
		uint8_t minRange;
		uint8_t maxRange;
		SPI_PackLrfConfig(&data, true, 1, 2, 3);
		printf("0x%X\n", data);
		SPI_UnpackLrfConfig(&laserPointer, &cmFreq, &minRange, &maxRange, data);
		printf("laser: %d, cmFreq: 0x%x, minRange: 0x%x, maxRange: 0x%x\n", laserPointer, cmFreq, minRange, maxRange);
	}

	{
		bool min;
		bool max;
		bool cm;
		SPI_PackLrfConfigStatus(&data, 1, 0, 1);
		printf("0x%X\n", data);
		SPI_UnpackLrfConfigStatus(&min, &max, &cm, data);
		printf("min: %d, max: 0x%x, cm: 0x%x\n", min, max, cm);
	}
    return 0;
}

void SPI_PackLrfProbeStatus(uint32_t *dst, bool probeFlag, uint8_t *deviceName3Chars)
{
	int i = LRF_DEVICE_NAME_LEN;
	*dst = probeFlag << 24;
	for (i=0; i<LRF_DEVICE_NAME_LEN; i++) {
		*dst |= deviceName3Chars[i] << 8*(LRF_DEVICE_NAME_LEN-1-i);
	}
}

void SPI_UnpackLrfProbeStatus(bool *probeFlag, uint8_t *deviceName3Chars, uint32_t src)
{
	int i = LRF_DEVICE_NAME_LEN;
	*probeFlag = (src>>24) & 1;
	for (i=0; i<LRF_DEVICE_NAME_LEN; i++) {
		deviceName3Chars[i] = src >> 8*(LRF_DEVICE_NAME_LEN-1-i);
	}
}

void SPI_PackLrfConfig(uint32_t *dst, bool laserPointer, uint8_t cmFreq, uint8_t minRange, uint8_t maxRange)
{
	*dst = maxRange;
	*dst |= minRange << 8;
	*dst |= (cmFreq&7) << 16;
	*dst |= (laserPointer) << 19;
}

void SPI_UnpackLrfConfig(bool *laserPointer, uint8_t *cmFreq, uint8_t *minRange, uint8_t *maxRange, uint32_t src)
{
	*maxRange = src & 0xff;
	*minRange = (src>>8) & 0xff;
	*cmFreq = (src>>16) & 7;
	*laserPointer = (src>>19) & 1;
}

void SPI_PackLrfConfigStatus(uint32_t *dst, bool minRangeFlag, bool maxRangeFlag, bool cmFlag)
{
	*dst = cmFlag&1;
	*dst |= (maxRangeFlag&1) << 1;
	*dst |= (minRangeFlag&1) << 2;
}

void SPI_UnpackLrfConfigStatus(bool *minRangeFlag, bool *maxRangeFlag, bool *cmFlag, uint32_t src)
{
	*cmFlag = src&1;
	*maxRangeFlag = (src>>1)&1;
	*minRangeFlag = (src>>2)&1;
}
