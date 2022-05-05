#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define MAG_AVERAGE_NUM_DATA_BIT	28
#define MAG_AVERAGE_NUM_DATA_MASK	0xfffffff

static int MagPackAvgData(uint8_t size, int avg);
static int UnpackMagData(int *ret, uint32_t data);

int main()
{
    int ret;
    int packedData;
    int size;


    packedData = MagPackAvgData(2, 0x4);
    printf("0x%08x\n", packedData);

    packedData = MagPackAvgData(9, 0xa000123);
    printf("0x%08x\n", packedData);

    size = UnpackMagData(&ret, packedData);
    printf("0x%08x(%d)\n", ret, size);
    return 0;
    
}

static int UnpackMagData(int *ret, uint32_t data)
{
    int size;
    size = data >> MAG_AVERAGE_NUM_DATA_BIT;
    *ret = data & MAG_AVERAGE_NUM_DATA_MASK;
    if ( (*ret & (1<<(MAG_AVERAGE_NUM_DATA_BIT-1))))
        *ret |= 0xf0000000;

    return size;
}

static int MagPackAvgData(uint8_t size, int avg)
{
	int ret;

	ret = avg & MAG_AVERAGE_NUM_DATA_MASK;
	ret |= (size & 0xf) << MAG_AVERAGE_NUM_DATA_BIT;

	return ret;
}