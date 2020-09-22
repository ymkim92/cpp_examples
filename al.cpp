#include <iostream>
#include <vector>
using namespace std;

bool Check(vector<int>& A)
{
    if (A.size() <= 1)
        return false;

    int i;
    for (i=1; i<A.size(); i++)
    {
        if (A[i-1] == A[i]) 
            return false;
        if (A[i-1] > A[i]) 
            break;
    }

    if (i >= A.size())
        return false;

    if (i == A.size()-1)
        return true;

    if (i == 1)
        return false;

    for (int j=i+1; j<A.size(); j++)
    {
        if (A[i-1] == A[i]) 
            return false;
        if (A[j-1] < A[j])
            return false;
    }

    return true;
}

int main()
{
    // vector<int> A; // false
    // vector<int> A{0}; // false
    // vector<int> A{1, 2, 3}; // false
    // vector<int> A{2, 1}; // false
    vector<int> A{14,82,89,84,79,70,70,68,67,66,63,60,58,54,44,43,32,28,26,25,22,15,13,12,10,8,7,5,4,3}; // true
    // vector<int> A{1, 2, 3, 1}; // true
    // vector<int> A{1, 2, 3, 1, 2}; // false

    cout << Check(A) << endl;
    return 0;
}
