#include <stdio.h>
#include <stdint.h>
#include <string.h>

void working()
{
    double dData = -2.1;
    printf("size of double = %ld\n", sizeof(double));

    printf("double = %lf\n", dData);
    uint8_t data[8];
    *((double *)data) = dData;
    printf("bytes ret = %02x %02x %02x %02x %02x %02x %02x %02x \n", data[0], data[1], 
                data[2], data[3], data[4], data[5], data[6], data[7]);

    double dData2;
    dData2 = *((double *)data);
    printf("double2 = %lf\n", dData2);
}

void working_pointer()
{
    double dData = -2.1f;
    printf("double = %f\n", dData);

    uint64_t *ui64Data;
    ui64Data = (uint64_t *)&dData;
    printf("uint64_t ret = 0x%lx\n", *ui64Data);

    printf("double = %lf\n", dData);
}

void working_memcpy()
{
    float fData = -2.0f;
    int16_t i16Data = fData;
    printf("size of float = %ld\n", sizeof(float));

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
    // printf("convert float to bytes: memcpy\n");
    // working_memcpy();
    // printf("-----------------------------------------\n");
    printf("convert float to bytes: pointer\n");
    working_pointer();
    printf("-----------------------------------------\n");
    printf("convert double to bytes: another pointer\n");
    working();
    return 0;
}