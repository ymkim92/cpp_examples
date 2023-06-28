#include <stdio.h>
#include <stdint.h>

#define AXIS_NUM 3
typedef struct
{
    uint32_t m_timestamp;
    uint8_t m_gpsFix;
    uint8_t m_numSatsUsedInSolution[8];
    uint8_t m_reserved;
    uint16_t m_reserved2;
    float m_pdop;
    float m_stdOfPosition[AXIS_NUM];
    float m_stdOfVelocity[AXIS_NUM];
    double m_positionInMetre[AXIS_NUM];
    double m_velocityInMetrePerSec[AXIS_NUM];
} GnssData_t;

int main()
{
    printf("sizeof struct: %ld\n", sizeof(GnssData_t));
}