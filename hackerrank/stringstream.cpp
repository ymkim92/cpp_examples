// https://www.hackerrank.com/challenges/c-tutorial-stringstream/problem
#include <sstream>
#include <vector>
#include <iostream>
using namespace std;

vector<int> parseInts(string str) {
    vector<int> ret;
    int value;
    char ch;

    stringstream ss(str);
    ss << str;

    while (!ss.eof())
    {
        ss >> value >> ch;
        ret.push_back(value);
    }
    return ret;
}

int main() {
    string str;
    cin >> str;
    vector<int> integers = parseInts(str);
    for(int i = 0; i < integers.size(); i++) {
        cout << integers[i] << "\n";
    }
    
    return 0;
}
