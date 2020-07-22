#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <set>
#include <algorithm>
using namespace std;

void PrintValueExistInSet(set<int> &s, int value)
{
    set<int>::iterator itr; 

    itr = s.find(value);
    if (itr != s.end())
    {
        cout << "Yes" << endl;
    }
    else
    {
        cout << "No" << endl;
    }
}

int main() {
    int q, c, val;
    set<int> s;
    cin >> q;
    
    for (int i=0; i<q; i++)
    {
        cin >> c >> val;
        if (c==1)
            s.insert(val);
        else if (c==2)
            s.erase(val);
        else if (c==3)
            PrintValueExistInSet(s, val);
    }
    
    return 0;
}



