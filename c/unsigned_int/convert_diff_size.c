#include <stdio.h>
#include <stdint.h>

void PrintNumberU32(uint32_t data)
{
    printf("0x%x\n", data);
}

int main()
{
    float fData = -2.0f;
    int16_t i16Data = fData;

    PrintNumberU32(i16Data);
    return 0;
}