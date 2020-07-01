#include <stdio.h>

struct data
{
    float accl[3];
};

struct data imu;

void IncreaseByOne(float *accl)
{
    int i;
    for (i=0; i<3; i++)
    {
        accl[i] += 1.0;
    }
}

int main()
{
    imu.accl[0] = 1.0;
    imu.accl[1] = 2.0;
    imu.accl[2] = 3.0;

    IncreaseByOne(imu.accl);

    printf("0: %f, 1: %f, 2: %f\n", 
        imu.accl[0],
        imu.accl[1],
        imu.accl[2]
        );
}