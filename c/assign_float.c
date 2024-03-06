#include <stdio.h>

int main()
{
    float a = 0xF2345678;
    printf("%ld\n", sizeof(float));
    printf("%lf\n", a);
    return 0;
}