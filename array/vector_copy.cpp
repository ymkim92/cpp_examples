
// resizing of the vector 
#include <iostream> 
#include <vector> 
  
using namespace std; 
  
int main() 
{ 
    vector<int> vec {1,2,3,4}; 
    vector<int> vec2;

    vec2 = vec;
  
    // displaying the contents of the 
    // vector after resizing 
    for (int i = 0; i < vec2.size(); i++) 
        cout << vec2[i] << " "; 
  
    cout << endl;

    return 0; 
} 
