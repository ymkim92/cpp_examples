#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


int main() {
    int n, q, i, j;

    cin >> n >> q;
    vector<int> k(n);
    vector<vector<int>> a(n);

    for (i=0; i<n; i++)
    {
        cin >> k[i];
        for (j=0; j<k[i]; j++)
        {
            int value;
            cin >> value;
            a[i].push_back(value);
        }
    }

    for (i=0; i<q; i++)
    {
        int x, y;
        cin >> x >> y;
        cout << a[x][y] << endl;
    }
    return 0;
}