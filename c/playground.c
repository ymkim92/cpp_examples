#include <stdio.h>

#define MAG_DATA_SENSITIVITY 16384.0f
#define MAG_AVG_MULTIPLIER 32
#define MAG_DATA_MG(raw) ((raw) * 1000 / (MAG_DATA_SENSITIVITY * MAG_AVG_MULTIPLIER))


int main()
{
    printf("%lf\n", MAG_DATA_MG(3031));
    return 0;
}