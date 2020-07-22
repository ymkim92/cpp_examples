#include <cmath>
#include <cstdio>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
using namespace std;

void AddMark(map<string, int> &a_map, string &a_student, int a_mark)
{
    int new_mark = a_mark;
    map<string, int>::iterator itr = a_map.find(a_student);
    if (itr != a_map.end())
    {
        new_mark = a_map[a_student] + a_mark;
        a_map.erase(a_student);
    }
    a_map.insert (  make_pair(a_student, new_mark)   );
}

void PrintMark(map<string, int> &a_map, string &a_student)
{
    map<string, int>::iterator itr = a_map.find(a_student);
    if (itr != a_map.end())
    {
        cout << a_map[a_student] << endl;
    }
    else
    {
        cout << 0 << endl;
    }
}

int main() {
    int q, cmd, mark;
    map<string, int> m;
    string name;

    cin >> q;

    for (int i=0; i<q; i++)
    {
        cin >> cmd >> name;
        if (cmd == 1)
        {
            cin >> mark;
            AddMark(m, name, mark);
        }
        else if (cmd ==2)
        {
            m.erase(name);
        }
        else if (cmd == 3)
            PrintMark(m, name);
    }
    return 0;
}