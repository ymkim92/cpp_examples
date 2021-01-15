#include <iostream>
#include <vector>

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        if ((head != NULL) && (head->next != NULL))
        {
            ListNode* newHead;
            ListNode* next;
            newHead = head->next;
            next = head->next->next;
            newHead->next = head;
            head->next = next;
            head = newHead;
            head->next->next = swapPairs(head->next->next);
        }
        return head;
        
        
    }
};

int main()
{
    Solution s = Solution();

    ListNode* head;

    ListNode n4(4);
    ListNode n3(3, &n4);
    ListNode n2(2, &n3);
    ListNode n1(1, &n2);

    head = &n1;
    head = s.swapPairs(head);
        // {
        //     ListNode* newHead;
        //     ListNode* next;
        //     newHead = head->next;
        //     next = head->next->next;
        //     newHead->next = head;
        //     head->next = next;
        //     head = newHead;
        // }

    int cnt = 0;
    while (head != NULL)
    {
        cout << head->val << endl;
        head = head->next;
        cnt += 1;
        if (cnt>10)
            break;
    }
    // for (auto i: st)
    //     cout << i << ",";
    // cout << endl;
    return 0;
}