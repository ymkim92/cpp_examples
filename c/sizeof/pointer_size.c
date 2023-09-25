#include <stdio.h>

int main()
{
    char input[1];
    size_t inputSize = sizeof(input);

    char *output = NULL;
    size_t outputSize = sizeof(output);

    printf("input size = %ld\n", inputSize);
    printf("output size = %ld\n", outputSize);
    return 0;
}