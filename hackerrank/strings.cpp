// https://www.hackerrank.com/challenges/c-tutorial-strings/problem
#include <iostream>
#include <string>
using namespace std;

int main() {
	// Complete the program
    string str1, str2;
    char ch;
    cin >> str1;
    cin >> str2;

    cout << str1.size() << " " << str2.size() << endl;
    cout << str1 + str2 << endl;
    ch = str1[0];
    str1[0] = str2[0];
    str2[0] = ch;
    cout << str1 << " " << str2 << endl;

  
    return 0;
}