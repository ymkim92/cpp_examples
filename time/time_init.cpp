#include <chrono>
#include <iostream>

using namespace std::chrono;

int main()
{
    time_point<steady_clock> now;

    if (now == steady_clock::time_point())
    {
        std::cout << "invalid" << std::endl;
    }
    
    now = steady_clock::now();

    if (now != steady_clock::time_point())
    {
        std::cout << "Vaild" << std::endl;
    }
    
    return 0;
}