// g++ avg.cpp  -std=c++11

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

const double LASER_METER_HEIGHT_MULTIPLIER      { 10000.0 };
const uint16_t LASER_HEIGHT_MASK                { 0x3FFF };
const uint16_t LASER_HEIGHT_IS_NEGATIVE         { 0x2000 };
const uint16_t LASER_HEIGHT_MAKE_NEGATIVE       { 0xC000 };
const uint16_t LASER_STATUS_MASK                { 0x8000 };
const uint16_t NUM_LASER_DATA                   { 5 };

bool ProcessFiveData(double *a_avgData);
double ConvertLaserDataUnitToMeter(uint16_t a_avgData);
double GetAvgData(double *data);

bool m_onLaser;
int16_t m_laserData;

int main()
{
    return 0;
}

bool ProcessFiveData(double *a_avgData)
{
    static int dataIndex = 0;
    static double data[NUM_LASER_DATA];
    static bool prevOnLaser;

    double distance = ConvertLaserDataUnitToMeter(m_laserData);
    if (dataIndex == 0)
    {
        data[0] = distance;
        prevOnLaser = m_onLaser;
        dataIndex = 1;
        return false;
    }

    if (prevOnLaser == false) 
    {
        prevOnLaser = m_onLaser;
        if (m_onLaser == false)   
        {
            data[dataIndex] = distance;
            dataIndex += 1;
            if (dataIndex == NUM_LASER_DATA)
            {
                *a_avgData = 0;
                dataIndex = 0;
                return true;
            }
            return false;
        }
        else
        {
            data[0] = distance;
            dataIndex = 0;
            return false;
        }
    }
    else
    {
        prevOnLaser = m_onLaser;
        if (m_onLaser == true)   
        {
            data[dataIndex] = distance;
            dataIndex += 1;
            if (dataIndex == NUM_LASER_DATA)
            {
                *a_avgData = GetAvgData(data);
                dataIndex = 0;
                return true;
            }
            return false;
        }
        else
        {
            data[0] = distance;
            dataIndex = 0;
            return false;
        }
    }

}

double ConvertLaserDataUnitToMeter(uint16_t a_avgData)
{
    uint16_t ui16Distance;

    m_onLaser = ((a_avgData & LASER_STATUS_MASK) == 0);  
    ui16Distance = a_avgData & LASER_HEIGHT_MASK;
    if ((ui16Distance & LASER_HEIGHT_IS_NEGATIVE) != 0)
        ui16Distance |= LASER_HEIGHT_MAKE_NEGATIVE;
    return static_cast<double>((int16_t)ui16Distance)/ LASER_METER_HEIGHT_MULTIPLIER;
}

double GetAvgData(double *data)
{
    double sum = 0;
    // string strData;
    for (int i=0; i<NUM_LASER_DATA; i++)
    {
        // strData += to_string(data[i]) + " ";
        sum += data[i];
    }
    // logger()->info(strData);
    return sum/NUM_LASER_DATA;
}