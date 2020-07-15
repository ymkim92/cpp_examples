//https://www.hackerrank.com/challenges/vector-sort/problem
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int n;
    int inNum;
    vector<int> numbers;

    cin >> n;
    for (int i=0; i<n; i++)
    {
        cin >> inNum;
        numbers.push_back(inNum);
    }
    sort(numbers.begin(), numbers.end());

    for (int i=0; i<n; i++)
    {
        cout << numbers[i] << " ";
    }
    cout << endl;
    return 0;
}