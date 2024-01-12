#include <stdio.h>

int main()
{
    char string[5];
    int len = snprintf(string, 5, "abc");
    printf("len=%d %s\n", len, string);

    len += snprintf(string + len, 5-len, "123");
    printf("len=%d %s\n", len, string);
    if (len > 5) {
        return 0;
    }

    len += snprintf(string + len, 5-len, "xyz");
    printf("len=%d %s\n", len, string);
    return 0;
}