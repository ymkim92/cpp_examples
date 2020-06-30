#include <stdio.h>

int main()
{
    int x = 5;
    int &lref = x;  // compilation error
    return 0;
}