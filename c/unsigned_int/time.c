#include <stdio.h>
#include <stdint.h>

int main()
{
    uint8_t ui8StartTime;
    uint8_t ui8Now;
    uint8_t ui8Diff;

    ui8StartTime = 254;
    ui8Diff = 3 - ui8StartTime;
    printf("%u, %u\n", ui8Diff, 3 - ui8StartTime);
    return 0;
}