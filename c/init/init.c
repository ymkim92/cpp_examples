#include <stdio.h>
#include <stdint.h>
#include <assert.h>

typedef struct {
    float a[3];
    uint16_t b[3];
} data_t;
int main()
{
    data_t data = {}; // same with { 0 }
    for (int i=0; i<3; i++) {
        assert(data.a[i] == 0.0f);
        assert(data.b[i] == 0);
    }
    return 0;
}