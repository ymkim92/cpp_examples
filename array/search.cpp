// $ g++ search.cpp -std=c++17

#include <iostream>
#include <iterator>

int array[]{ 4, 6, 7, 3, 8, 2, 1, 9, 5 };

int main()
{
    int value{3};

    for (int i=0; i<std::size(array); i++)
    {
        if (array[i] == value)
        {
            std::cout << i << std::endl;
            break;
        }
    }
}