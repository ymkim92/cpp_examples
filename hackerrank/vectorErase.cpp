// https://www.hackerrank.com/challenges/vector-erase/problem?h_r=next-challenge&h_v=zen
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

void printVector(auto v)
{
    for (int i: v)
    {
        cout << i << " ";
    }
    cout << endl;
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int n, value;
    vector<int> v;
    int e1;
    int e21, e22;
    cin >> n;
    for (int i=0; i<n; i++)
    {
        cin >> value;
        v.push_back(value);
    }
    cin >> e1;
    cin >> e21;
    cin >> e22;

    // n = 6;
    // v.push_back(1);
    // v.push_back(4);
    // v.push_back(6);
    // v.push_back(2);
    // v.push_back(8);
    // v.push_back(9);
    // e1 = 2;
    // e21 = 2;
    // e22 = 4;
    v.erase(v.begin()+e1-1);
    v.erase(v.begin()+e21-1, v.begin()+e22-1);

    cout << v.size() << endl;
    printVector(v);
    return 0;
}