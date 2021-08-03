#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <cmath>

using namespace std;

typedef float float32_t;
typedef double float64_t;

std::vector<std::string> m_fields;

// GGA message
enum Fix
{
    FixNone = 0,
    FixGps = 1,
    FixDgps = 2,
    FixUnknown = -1 
};

bool m_ok;
uint8_t hour;
uint8_t minute;
float32_t sec;
float64_t lat;
float64_t lon;
Fix fix;
uint32_t numSats;
float32_t hdop;
float32_t alt;
float32_t ellipsoidHeight;
float32_t dgpsAge;
std::string dgpsStationId;


void extractSource(const char* buf, unsigned int length);
void extractFields(const char* buf, unsigned int length);
void DecodeNmeaGga(const char* buf, unsigned int length);
bool checksum(const char* buf, unsigned int length);
static double degreesMinutesToDegrees(double dm);

int main()
{
    const char testGpgga[] =
        "$GPGGA,001304.70,0400.00256616,K,00000.00213362,E,1,3,,1.582,M,0.0,M,,*4D";
        // "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
        // "$GPGGA,,4807.038,*47\r\n";
        // "$GPGGA,123519,   4807.038,     N,01131.000,     E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
        // "$GPGGA,060459.30,0400.00200327,N,00000.00222622,E,1, 3,   ,1.790,M,0.0, M,,*47";
        //  0      1         2             3 4              5 6 7  8   9     0 1    23 4

    extractFields(testGpgga, strlen(testGpgga));
    for (int i = 0; i < m_fields.size(); i++) 
        cout << m_fields[i] << " "; 
  
    cout << endl; 

    DecodeNmeaGga(testGpgga, strlen(testGpgga));
    cout << "lat:" << lat << endl;
    cout << "fix:" << fix << endl;

}

void extractSource(const char* buf, unsigned int length)
{

}

void extractFields(const char* buf, unsigned int length)
{
    m_fields.clear();
    length -= 5; // don't consider the checksums;

    const char* bufEnd = &buf[length];

    const char* current  = buf;

    while (current < bufEnd)
    {
        if (',' == *current)
        {
            int fieldLength = current - buf;
            if (fieldLength == 0)
            {
                m_fields.push_back("");
            }
            else
            {
                m_fields.push_back(std::string(buf, fieldLength));
            }
            ++current;
            buf = current;
        }
        else
            ++current;
    }
    // catch the last field
    m_fields.push_back(std::string(buf, bufEnd - buf));
}

void DecodeNmeaGga(const char* buf, unsigned int length)
{
    m_ok = false;

    // if (true == checksum(buf, length))
    {
        extractSource(buf, length);
        extractFields(buf, length);

        if(13 <= m_fields.size())
        {
            if(6 <= m_fields[1].size())
            {
                // hour
                hour = static_cast<uint8_t>(0xffL & strtoul(m_fields[1].substr(0,2).c_str(), NULL, 10));

                // minutes
                minute = static_cast<uint8_t>(0xffL & strtoul(m_fields[1].substr(2,2).c_str(), NULL, 10));

                // seconds
                sec = strtof(m_fields[1].substr(4,m_fields[1].length() - 4).c_str(), NULL);

            }
            else
            {
                hour = 0;
                minute = 0;
                sec = 0.0f;
            }

            // Latitude
            lat = degreesMinutesToDegrees(strtod(m_fields[2].c_str(), NULL));
            if (m_fields[3] == "S")
            {
                lat = -lat;
            }

            // longitude
            lon = degreesMinutesToDegrees(strtod(m_fields[4].c_str(), NULL));
            if (m_fields[5] == "W")
            {
                lon = -lon;
            }

            // Fix type
            switch (m_fields[6][0])
            {
                case '0':
                    fix = FixNone;
                    break;
                case '1':
                    fix = FixGps;
                    break;
                case '2':
                    fix = FixDgps;
                    break;
                default:
                    fix = FixUnknown;
                    break;
            }

            // Number of satellites
            numSats = strtoul(m_fields[7].c_str(), NULL, 10);

            // hdop
            hdop = strtof(m_fields[8].c_str(), NULL);

            // altitude
            alt = strtof(m_fields[9].c_str(), NULL);

            // Ellipsoid height
            ellipsoidHeight = strtof(m_fields[11].c_str(), NULL);

            if(14 <= m_fields.size())
            {
                // dgpsAge 0 if the field is not present
                dgpsAge = strtof(m_fields[13].c_str(), NULL);
            }
            else 
                dgpsAge = 0;

            if(16 <= m_fields.size())
            {
                dgpsStationId = m_fields[15];
            }
            else
            {
                // empty string if the field is not present
                dgpsStationId = "";
            }

            m_ok = true;
        }
    }
}

bool checksum(const char* buf, unsigned int length)
{
    // checksum is between $ and * not inclusive
    // Framer guarantees that first character is $

    uint8_t sum = 0;
    unsigned int i;
    for (i = 1; (i < length) && ('*' != buf[i]); i++)
    {
        sum = sum ^ static_cast<uint8_t>(buf[i]);
    }

    unsigned long int inSum = strtoul(&buf[i+1], NULL, 16);
    uint8_t inSumByte = static_cast<uint8_t>(0xff & inSum);

    return (sum == inSumByte);
}

static double degreesMinutesToDegrees(double dm)
{
    double degrees = trunc(dm / 100.0);
    double minutes = (dm - (degrees * 100.0)) / 60.0;
    return degrees + minutes;
}