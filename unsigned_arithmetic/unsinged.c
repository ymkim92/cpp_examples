#include <stdio.h>
#include <stdint.h>

int main()
{
    uint8_t a, b, c;
    a = 254;
    b = 3;
    c = b - a;
    printf("b - a = %u\n", b-a);
    printf("c = %u\n", c);
    return 0;
}

/*
$ ./a.out
b - a = 4294967045
c = 5
*/