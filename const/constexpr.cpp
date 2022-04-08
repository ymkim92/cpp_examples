#include <iostream>

constexpr int fibonacci(const int n) 
{
    return n == 1 || n == 2 ? 1 : fibonacci(n-1) + fibonacci(n-2);
}

int main()
{
    int i = 5;
    std::cout << fibonacci(10) << std::endl;
    std::cout << fibonacci(i) << std::endl;

    // Segmentation fault (core dumped)
    for (i=0; i<5; i++)
    {
        std::cout << fibonacci(i) << std::endl;
    }
}