/*
3 1
set 1 2
get 1
get 2
*/

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
using namespace std;

struct Node{
   Node* next;
   Node* prev;
   int value;
   int key;
   Node(Node* p, Node* n, int k, int val):prev(p),next(n),key(k),value(val){};
   Node(int k, int val):prev(NULL),next(NULL),key(k),value(val){};
};

class Cache{
   
   protected: 
   map<int,Node*> mp; //map the key to the node in the linked list
   int cp;  //capacity
   Node* tail; // double linked list tail pointer
   Node* head; // double linked list head pointer
   virtual void set(int, int) = 0; //set function
   virtual int get(int) = 0; //get function

};


// Put the code here
class LRUCache: public Cache
{
private:
    void AddFoundNodeToHead(Node *n);
    void RemoveTail();
    void AddNodeToHead(Node *n);
public:
    LRUCache(int c) {cp = c;} 
    virtual int get(int key);
    virtual void set(int key, int value);

    void PrintList();
    void PrintNode(Node *node);
};

int LRUCache::get(int key)
{
    auto it = mp.find(key);
    if (it != mp.end()) // found
    {
        // return mp[key]->value;
        return it->second->value;    // IMPORTANT!!!!! this one is much faster than map search
    }

    return -1;
}

void LRUCache::set(int key, int value)
{
    if (cp <= 0)
        return;
    if (mp.empty())   
    {
        Node* node = new Node(key, value);
        head = tail = node;
        mp[key] = node;
        return;
    }

    auto it = mp.find(key);
    if (it != mp.end()) // found
    {
        it->second->value = value;
        AddFoundNodeToHead(it->second);
        // AddFoundNodeToHead(mp[key]->value);
    }
    else // not found
    {
        AddNodeToHead(new Node(key, value));
        if (mp.size() > cp)
            RemoveTail();
    }
}

void LRUCache::AddFoundNodeToHead(Node *n)
{
    if (n->prev == NULL) // n is head
    {
        return;
    }

    n->prev->next = n->next;
    n->prev = NULL;

    if (n->next != NULL) // n is tail
    {
        n->next->prev = n->prev;
    }
    n->next = head;
    head->prev = n;
    head = n;


}

void LRUCache::RemoveTail()
{
    tail = tail->prev;
    mp.erase(tail->next->key);
    delete tail->next;
    tail->next = NULL;
}

void LRUCache::AddNodeToHead(Node *n)
{
    head->prev = n;
    n->next = head;
    head = n;
    mp[n->key] = n;
}

// debugging
void LRUCache::PrintList()
{
    Node *node = head;
    while (node != NULL)
    {
        PrintNode(node);
        // printf("[%p]\n", node->next);
        // cout << "[" << node->next << "]" << endl;
        node = node->next;
    }
    cout << endl;
}

void LRUCache::PrintNode(Node *node)
{
    cout << "(" << node->key << ", " << node->value << ") ";
}

int main() {
   int n, capacity,i;
   cin >> n >> capacity;
   LRUCache l(capacity);
   for(i=0;i<n;i++) {
      string command;
      cin >> command;
      if(command == "get") {
         int key;
         cin >> key;
         cout << l.get(key) << endl;
      } 
      else if(command == "set") {
         int key, value;
         cin >> key >> value;
         l.set(key,value);
        //  l.PrintList();
      }
   }
   return 0;
}
