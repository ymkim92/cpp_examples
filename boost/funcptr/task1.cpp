#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include "task1.h"

functionPtr funcTest;

void workerTask()
{
    boost::posix_time::seconds workTime(2);
    std::cout << "Task: running" << std::endl;

    boost::this_thread::sleep(workTime);
    if (funcTest != NULL)
        std::cout << "Sum of 2 and 3:" 
            << funcTest(2, 3) << std::endl;
    std::cout << "Task: finished" << std::endl;
}

void RegisterFuncPtr(functionPtr func)
{
    funcTest = func;
}