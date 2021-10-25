#include <stdint.h>
#include <iostream>

using namespace std;
int main()
{
    float f[] = {1234.0f, -1234.0f, 0x7fff, 0x8000};

    int16_t ii;

    for (int i=0; i<4; i++)
    {
        ii = f[i];
        cout << f[i] << ": " << ii << endl;
    }
    return 0;
}