#include <iostream>
#include <boost/thread.hpp>

#include "task1.h"

int sum(int a, int b)
{
    return a+b;
}

int mul(int a, int b)
{
    return a*b;
}

int main()
{
    std::cout << "main: startup" << std::endl;
    boost::thread workerThread(workerTask);

    std::cout << "main: waiting for thread" << std::endl;
    RegisterMulFuncPtr(mul);
    workerThread.join();

    std::cout << "main: done" << std::endl;
    return 0;
}