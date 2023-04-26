#include <stdio.h>
#include <stdint.h>
#include <assert.h>

// v: value, i: index 
// if v is 0x12345678, index 0 of v is 0x12
#define GET_BYTE_FROM_INT32_BIG_ENDIAN(v, i) (((v) >> (3-(i))*8) & 0xff )
#define GET_BYTE_FROM_INT16_BIG_ENDIAN(v, i) (((v) >> (1-(i))*8) & 0xff )

// p: pointer to bytes
#define GET_INT32_FROM_BYTES_BIG_ENDIAN(p) (((p)[0] << 24) | ((p)[1] << 16) | ((p)[2] << 8) | (p)[3])
#define GET_INT16_FROM_BYTES_BIG_ENDIAN(p) (((p)[0] << 8 | (p)[1]))

int main()
{
    {
        uint32_t a = 0x12345678;
        assert(GET_BYTE_FROM_INT32_BIG_ENDIAN(a, 1+1) == 0x56);
        printf("%02X\n", GET_BYTE_FROM_INT32_BIG_ENDIAN(a, 1+1) );

        uint8_t data[4];
        int i;
        for (i=0; i<4; i++)
        {
            data[i] = GET_BYTE_FROM_INT32_BIG_ENDIAN(a, i);
            printf("0x%x ", data[i]);
        }
        printf("\n");

        uint32_t b = GET_INT32_FROM_BYTES_BIG_ENDIAN(data);
        printf("0x%X\n", b);

        printf("** %02X\n", GET_INT16_FROM_BYTES_BIG_ENDIAN(&data[1]) );

    }

    {
        uint16_t a = 0xabcd;
        assert(GET_BYTE_FROM_INT16_BIG_ENDIAN(a, 1) == 0xcd);
        printf("%02X\n", GET_BYTE_FROM_INT32_BIG_ENDIAN(a, 1) );

        uint8_t data[2];
        int i;
        for (i=0; i<2; i++)
        {
            data[i] = GET_BYTE_FROM_INT16_BIG_ENDIAN(a, i);
            printf("0x%x ", data[i]);
        }
        printf("\n");

        uint16_t b = GET_INT16_FROM_BYTES_BIG_ENDIAN(data);
        printf("0x%X\n", b);
    }


    return 0;
}