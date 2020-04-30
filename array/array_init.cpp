#include <iostream>

int main()
{
    // int a[2](5); // get error 
    int a[2]{5};

    for (int i=0; i<2; i++)
    {
        std::cout << a[0] << ", " << a[1] << std::endl;
        return 0;
    }
}