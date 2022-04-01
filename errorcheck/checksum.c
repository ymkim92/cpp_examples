#include <stdio.h>
#include <stdint.h>

static uint8_t GetLrfChecksum(uint8_t *buf, uint16_t size);

int main()
{
    uint8_t buf[] = {0xc3};

    printf("0x%x\n", GetLrfChecksum(buf, sizeof(buf)));
    return 0;
}

static uint8_t GetLrfChecksum(uint8_t *buf, uint16_t size)
{
	uint8_t sum=0;
	while (size > 0)
	{
		sum += *buf;
		buf += 1;
		size -= 1;
	}
	
	return sum ^ 0x50;
}