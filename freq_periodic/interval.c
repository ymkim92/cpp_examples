#include <stdio.h>
#include <stdint.h>

int main() 
{
    static uint32_t ui32NextSendTime = 0;
    uint32_t ui32Now = 0xffffff00;
    ui32NextSendTime = ui32Now;
    int i;

    for (i=0; i<1000; i++)
    {
        ui32Now += 10;
        if (ui32NextSendTime - ui32Now >= 200)
        {
            printf("%3d: %u - %u = %u\n", i, ui32NextSendTime, ui32Now, ui32NextSendTime - ui32Now);
            ui32NextSendTime += 200;
        }
    }
    return 0;
}
