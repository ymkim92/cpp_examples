#include <iostream>
#include <stdint.h>
#include <stdbool.h>

using namespace std;
// const double LASER_METER_HEIGHT_MULTIPLIER      { 10000.0 };

static constexpr uint8_t value2 = 0xfu;
static constexpr uint8_t length { 3u };
int16_t value3 = 15;
char name[3];

int main(int argc, char **argv)
{
    int16_t value;
    int ret;
    std::string m_lrf_device_name { "LRF" };
    char device_name[] = "LRF";
    value = 20;
    cout << value << endl;
    cout << unsigned(value2) << endl;
    cout << value3 << endl;

    name[0] = 'L';
    name[1] = 'R';
    name[2] = 'F';
    cout << std::string(name,3) << endl;
    ret = m_lrf_device_name.compare(0, length, std::string(device_name, length));
    cout << ret << endl;
    return 0;
}
