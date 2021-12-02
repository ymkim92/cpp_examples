#include <iostream>
#include <stdint.h>
#include <stdbool.h>

using namespace std;
// const double LASER_METER_HEIGHT_MULTIPLIER      { 10000.0 };

int main(int argc, char **argv)
{
    int16_t value;
    value = 20;
    if (value & 1)
        cout << "odd" << endl;
    else
        cout << "even" << endl;

    value = 25;
    if (value & 1)
        cout << "odd" << endl;
    else
        cout << "even" << endl;
    return 0;
}
