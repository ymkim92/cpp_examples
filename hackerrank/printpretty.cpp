#include <iostream>
#include <iomanip> 
using namespace std;

int main() {
	int T; cin >> T;
	cout << setiosflags(ios::uppercase);
	cout << setw(0xf) << internal;
	while(T--) {
		double A; cin >> A;
		double B; cin >> B;
		double C; cin >> C;

        cout.unsetf(ios::uppercase);
        cout << setw(0) << "0x" << hex << static_cast<long int>(A);
        cout << endl;
        cout.fill('_');
        cout.setf(ios::showpos);
        cout << std::fixed;
        cout << setprecision(2) << right << setw(15) << B << endl;
        cout.unsetf(ios::showpos);
        cout.setf(ios::uppercase);
        cout << std::scientific;
        cout << setprecision(9) << setw(15) << C << endl;
    }
    return 0;

}