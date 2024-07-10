#include <stdio.h>
#include <stdint.h>

int main()
{
    uint32_t milliVolt = 600;
    int16_t boardTemperature = (milliVolt - 500) / 10;
    printf("%d\n", boardTemperature);
    milliVolt = 400;
    boardTemperature = (int16_t)(milliVolt - 500) / 10;
    //                 ^^^^^^^^^ This is important
    printf("%d\n", boardTemperature);
    return 0;
}