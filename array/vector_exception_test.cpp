
// resizing of the vector 
#include <iostream> 
#include <vector> 
  
using namespace std; 
  
int main() 
{ 
    vector<int> vec {}; 
  
    vec.push_back(1); 
    vec.push_back(2); 
    vec.push_back(3); 
  
    for (int i = 0; i < vec.size(); i++) 
        cout << vec[i] << " "; 
    cout << endl;

    cout << vec.at(4) << endl;
    return 0; 
} 
