#include <stdio.h>

typedef enum {
        ARG1 = 2,
        ARG2 = 4
} Arg_t;

void foo(Arg_t a)
{
    printf("%d\n", a);
}

int main()
{
    foo(ARG2);
    foo(3);
    return 0;
}