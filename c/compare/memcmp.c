#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main()
{
    uint8_t p1[10];
    uint8_t p2[10];
    int i;

    for (i=0; i<5; i++)    
    {
        p1[i] = i;
        p2[i] = i;
    }
    p2[5] = 5;

    printf("%d\n", memcmp(p1, p2, 5));
    return 0;
}