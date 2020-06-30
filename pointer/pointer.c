#include <stdio.h>

int main()
{
	int x = 4;
    printf("%d\n", x);
    printf("%p\n", &x);
    // printf("%x\n", *x); // compilation error
 
	return 0;
}