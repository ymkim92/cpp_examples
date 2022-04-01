#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
 
void SPI_PackMagProbeStatus(uint32_t *dst, bool pIdStatus, uint8_t pId);
void SPI_UnpackMagProbeStatus(bool *pIdStatus, uint8_t *pId, uint32_t src);
void SPI_PackMagConfig(uint32_t *dst, bool autoSetReset, uint8_t cmFreq, uint8_t prdSet);
void SPI_UnpackMagConfig(bool *autoSetReset, uint8_t *cmFreq, uint8_t *prdSet, uint32_t src);
void SPI_PackMagConfigStatus(uint32_t *dst, bool autoSetReset, bool cmFreq, bool prdSet);
void SPI_UnpackMagConfigStatus(bool *autoSetReset, bool *cmFreq, bool *prdSet, uint32_t src);

int main()
{
	uint32_t data;
	{
		bool pIdStatus;
		uint8_t pId;

		SPI_PackMagProbeStatus(&data, false, 0x41);
		printf("0x%X\n", data);
		SPI_UnpackMagProbeStatus(&pIdStatus, &pId, 0xffffffff);
		printf("flag: %d, ID: 0x%x\n", pIdStatus, pId);

		SPI_UnpackMagProbeStatus(&pIdStatus, &pId, data);
		printf("flag: %d, ID: 0x%x\n", pIdStatus, pId);
	}
	{
		bool autoSetReset;
		uint8_t cmFreq;
		uint8_t prdSet;
		SPI_PackMagConfig(&data, true, 3, 5);
		printf("0x%X\n", data);
		SPI_UnpackMagConfig(&autoSetReset, &cmFreq, &prdSet, data);
		printf("flag: %d, cmFreq: 0x%x, prdSet: 0x%x\n", autoSetReset, cmFreq, prdSet);
	}

	{
		bool autoSetReset;
		bool cmFreq;
		bool prdSet;
		SPI_PackMagConfigStatus(&data, 1, 0, 1);
		printf("0x%X\n", data);
		SPI_UnpackMagConfigStatus(&autoSetReset, &cmFreq, &prdSet, data);
		printf("flag: %d, cmFreq: 0x%x, prdSet: 0x%x\n", autoSetReset, cmFreq, prdSet);

	}

    return 0;
}

void SPI_PackMagProbeStatus(uint32_t *dst, bool pIdStatus, uint8_t pId)
{
	*dst = pId;
	*dst |= pIdStatus << 8;
}

void SPI_UnpackMagProbeStatus(bool *pIdStatus, uint8_t *pId, uint32_t src)
{
	*pId = src&0xff;
	*pIdStatus = (src>>8) & 1;
}

void SPI_PackMagConfig(uint32_t *dst, bool autoSetReset, uint8_t cmFreq, uint8_t prdSet)
{
	*dst = prdSet&7;
	*dst |= (cmFreq&7) << 3;
	*dst |= autoSetReset << 6;
}

void SPI_UnpackMagConfig(bool *autoSetReset, uint8_t *cmFreq, uint8_t *prdSet, uint32_t src)
{
	*prdSet = src&7;
	*cmFreq = (src>>3)&7;
	*autoSetReset = (src>>6) & 1;
}

void SPI_PackMagConfigStatus(uint32_t *dst, bool autoSetReset, bool cmFreq, bool prdSet)
{
	*dst = prdSet&1;
	*dst |= (cmFreq&1) << 1;
	*dst |= (autoSetReset&1) << 2;
}

void SPI_UnpackMagConfigStatus(bool *autoSetReset, bool *cmFreq, bool *prdSet, uint32_t src)
{
	*prdSet = src&1;
	*cmFreq = (src>>1)&1;
	*autoSetReset = (src>>2) & 1;
}
