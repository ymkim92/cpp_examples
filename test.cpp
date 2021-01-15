#include <iostream>
#include <stdint.h>
#include <stdbool.h>

using namespace std;
const double LASER_METER_HEIGHT_MULTIPLIER      { 10000.0 };
const uint16_t LASER_HEIGHT_MASK                { 0x3FFF };
const uint16_t LASER_HEIGHT_IS_NEGATIVE         { 0x2000 };
const uint16_t LASER_HEIGHT_MAKE_NEGATIVE       { 0xC000 };
const uint16_t LASER_STATUS_MASK                { 0x8000 };

double ConvertLaserDataUnitToMeter(uint16_t a_avgData)
{
    int16_t i16Distance;
    bool m_onLaser;

    m_onLaser = ((a_avgData & LASER_STATUS_MASK) == 0);  
    i16Distance = a_avgData & LASER_HEIGHT_MASK;
    if ((i16Distance & LASER_HEIGHT_IS_NEGATIVE) != 0)
        i16Distance |= LASER_HEIGHT_MAKE_NEGATIVE;
    return static_cast<double>(i16Distance)/ LASER_METER_HEIGHT_MULTIPLIER;
}

int main(int argc, char **argv)
{
    int16_t i16Distance;
    uint16_t m_laserData = 0x8000;
    // uint16_t m_laserData = 8000;
    double m_laserHeight;
    bool m_onLaser;

        m_laserData = static_cast<uint16_t>(m_laserData);
        m_onLaser = ((m_laserData & LASER_STATUS_MASK) == 0);  //0 == On Laser, 1 == No Laser
        i16Distance = m_laserData & LASER_HEIGHT_MASK;
        if ((i16Distance & LASER_HEIGHT_IS_NEGATIVE) != 0)
            i16Distance |= LASER_HEIGHT_MAKE_NEGATIVE;
        double distance = static_cast<double>(i16Distance)/ LASER_METER_HEIGHT_MULTIPLIER;
        m_laserHeight = distance;

        cout << "OnLaser:" << m_onLaser << ", ";
        cout << "LaserHeight:" << m_laserHeight << ", ";

        m_laserHeight = ConvertLaserDataUnitToMeter(m_laserData);
        cout << "LaserHeight:" << m_laserHeight << endl;
    return 0;
}
