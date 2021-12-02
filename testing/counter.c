#include <stdio.h>
#include <stdint.h>

int main()
{
    uint8_t a, b, c;

    b = 236;
    for (a=0; a<200; a++)
    {
        c = a - b;
        if (c == 20)
        {
            printf("a:%u, b:%u\n", a, b);
            b = a;
        }
    }
    return 0;
}