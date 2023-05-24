#include <stdio.h>
#include <stdint.h>

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

int main()
{
    uint8_t err = 0x80;
    printf("%d \n", GetField(err, SERVO_HW_ERROR_MASK, SERVO_HW_ERROR_SHIFT));
    return 0;
}