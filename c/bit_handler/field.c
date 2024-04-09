#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define SERVO_HW_ERROR_SHIFT        7
#define SERVO_HW_ERROR_MASK         (1<<SERVO_HW_ERROR_SHIFT)

uint32_t GetField(uint32_t all, uint32_t mask, uint32_t shift)
{
    return (all & mask) >> shift;
}

uint32_t SetField(uint32_t all, uint32_t mask, uint32_t shift, uint32_t value)
{
    return (all & ~mask) | ((value << shift) & mask);
}

uint32_t SetBit(uint32_t all, uint32_t shift, uint32_t value)
{
    return SetField(all, 1 << shift, shift, value);
}

int main()
{
    uint8_t err = 0x80;
    printf("%d \n", GetField(err, SERVO_HW_ERROR_MASK, SERVO_HW_ERROR_SHIFT));
    uint32_t testAll = 0;
    testAll = SetBit(testAll, 2, 1);
    printf("testAll 0x%x\n", testAll);
    assert(testAll == 4);
    return 0;
}