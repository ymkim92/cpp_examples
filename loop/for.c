#include <stdio.h>

int GetNumbers()
{
    int value[] = {1, 2, 3, 0};
    static int i = 0;
    i += 1;
    if (i > 3) {
        return 0;
    }
    return value[i];
}

int main()
{
    for (int i = 0; (i = GetNumbers()) != 0;) {
        printf("%d\n", i);
    }
    return 0;
}