#include <iostream>
#include <boost/thread.hpp>

#include "task1.h"


int main()
{
    std::cout << "main: startup" << std::endl;
    boost::thread workerThread(workerTask);

    sum(1);
    std::cout << "main: waiting for thread" << std::endl;
    workerThread.join();


    std::cout << "main: done, sum = " << sum(0) << std::endl;
    return 0;
}