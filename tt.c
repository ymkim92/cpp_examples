#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define LRF_DEVICE_NAME_LEN 1

int main()
{
    uint32_t data = 2;
    if (data & 1 << LRF_DEVICE_NAME_LEN)
    {
        printf("CRC Error\n");
        return 0;
    }
}