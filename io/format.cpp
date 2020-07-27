#include <iostream>
#include <iomanip> // for setw
using namespace std;

int main()
{
    double A = 100.345;
    double B = 2006.008;
    double C = 2331.41592653498;

    cout << "0x" << hex << static_cast<int>(A) << endl;
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