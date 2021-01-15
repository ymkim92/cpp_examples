#include <iostream>
#include <vector>

using namespace std;
class Solution {
public:
    void helper(vector<char>& s, int left, int right)
    {
        char tmp;
        if (left >= right)
            return;
        tmp = s[right];
        cout << tmp << endl;
        s[right] = s[left];
        s[left] = tmp;
        helper(s, ++left, --right);
    }
    
    void reverseString(vector<char>& s) {
        cout << s.size() << endl;
        helper(s, 0, s.size()-1);
    }
};

int main()
{
    Solution s = Solution();

    vector<char> st;
    st.push_back('h');
    st.push_back('e');
    st.push_back('l');
    st.push_back('l');
    st.push_back('o');

    s.reverseString(st);

    // for (auto i: st)
    //     cout << i << ",";
    // cout << endl;
    return 0;
}