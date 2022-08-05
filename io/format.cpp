#include <iostream>
#include <iomanip> // for setw
using namespace std;

int main()
{
    double A = 100.345;
    double B = 2006.008;
    double C = 2331.41592653498;
    uint8_t num = 10;
    uint8_t buf[] = {1, 2, 10};

    cout << "0x" << hex << static_cast<int>(A) << endl;
    stringstream ss;
    for (uint8_t i=0; i<sizeof(buf); i++)
    {
        ss << setfill('0') << setw(2) << hex << static_cast<int>(buf[i]) << " ";
    }
    cout << ss.str() << endl;
    cout << setw(15) << B << endl;
    cout.fill('_');
    cout.setf(ios::showpos);
    cout << std::fixed;
    cout << setprecision(3) << setw(15) << B << endl;
    cout.unsetf(ios::showpos);
    cout << std::scientific;
    cout << setprecision(9) << setw(15) << C << endl;
    return 0;
}