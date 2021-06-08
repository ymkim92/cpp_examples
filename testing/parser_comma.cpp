#include <string>
#include <cstring>
#include <vector>
#include <iostream>

using namespace std;

std::vector<std::string> m_fields;

void ExtractFields(const char* buf, unsigned int length);

int main()
{
    const char testGpgga[] =
        // "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
        "$GPGGA,,4807.038,*47\r\n";
        // "$GPGGA,123519,   4807.038,     N,01131.000,     E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
        // "$GPGGA,060459.30,0400.00200327,N,00000.00222622,E,1, 3,   ,1.790,M,0.0, M,,*47";
        //  0      1         2             3 4              5 6 7  8   9     0 1    23 4

    // ExtractFields(testGpgga, sizeof(testGpgga) - 1);
    ExtractFields(testGpgga, strlen(testGpgga));
    for (int i = 0; i < m_fields.size(); i++) 
        cout << m_fields[i] << " "; 
  
    cout << endl; 

}

void ExtractFields(const char* buf, unsigned int length)
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