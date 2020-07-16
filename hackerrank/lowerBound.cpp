#include <cmath>
#include <cstdio>
#include <vector>
#include <map>
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

    map<int, int> m;
    {
        int i=0;
        for (int value: v)
        {
            i += 1;
            m.insert(pair<int, int>(value, i));    
        }

    }

    for (int i: qv)
    {
        if (m.find(i) != m.end())
        {
                cout << "Yes " << m.at(i) << endl;
        }
        else
        {
                cout << "No " << m.lower_bound(i)->first-1 << endl;
        }
    }
    return 0;
}
