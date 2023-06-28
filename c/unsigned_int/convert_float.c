#include <stdio.h>
#include <stdint.h>
#include <string.h>

void PrintNumberU32(uint32_t data)
{
    printf("0x%x\n", data);
}

void working()
{
    float fData = -2.0f;

    printf("float = %f\n", fData);
    uint8_t data[4];
    *((float *)data) = fData;
    printf("bytes ret = %02x %02x %02x %02x\n", data[0], data[1], data[2], data[3]);

    float fData2;
    fData2 = *((float *)data);
    printf("float2 = %f\n", fData2);
}

void working_pointer()
{
    float fData = -2.0f;
    printf("float = %f\n", fData);

    uint32_t *ui32Data;
    ui32Data = (uint32_t *)&fData;
    printf("uint32_t ret = 0x%x\n", *ui32Data);

    printf("float = %f\n", fData);
}

void working_memcpy()
{
    float fData = -2.0f;
    int16_t i16Data = fData;
    printf("size of float = %ld\n", sizeof(float));

    PrintNumberU32(i16Data);
    printf("float = %f\n", fData);

    uint32_t ui32Data;
    memcpy((uint8_t *)&ui32Data, (uint8_t *)&fData, 4);
    printf("memcpy ret = 0x%x\n", ui32Data);

    float fData2;
    memcpy((uint8_t *)&fData2, (uint8_t *)&ui32Data, 4);
    printf("float2 = %f\n", fData2);
}

int main()
{
    printf("convert float to bytes: memcpy\n");
    working_memcpy();
    printf("-----------------------------------------\n");
    printf("convert float to bytes: pointer\n");
    working_pointer();
    printf("-----------------------------------------\n");
    printf("convert float to bytes: another pointer\n");
    working();
    return 0;
}