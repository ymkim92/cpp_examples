#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


int main() {
    int n;
    vector<int> v;
    int q;
    vector<int> qv;

    if (1)
    {
        n = 8;
        v.push_back(1);
        v.push_back(1);
        v.push_back(2);
        v.push_back(2);
        v.push_back(6);
        v.push_back(9);
        v.push_back(9);
        v.push_back(15);
        q = 5;
        qv.push_back(1);
        qv.push_back(4);
        qv.push_back(9);
        qv.push_back(15);
        qv.push_back(20);
    }
    else
    {
        cin >> n;
        for (int i=0; i<n; i++)
        {
            int value;
            cin >> value;
            v.push_back(value);
        }
        cin >> q;
        for (int i=0; i<q; i++)
        {
            int value;
            cin >> value;
            qv.push_back(value);
        }
    }

    for (int i: qv)
    {
        int index = 0;
        for (int j: v)
        {
            index += 1;
            if (i == j)
            {
                cout << "Yes " << index << endl;
                break;
            }
            if (i < j)
            {
                cout << "No " << index << endl;
                break;
            }
        }
    }
    return 0;
}
