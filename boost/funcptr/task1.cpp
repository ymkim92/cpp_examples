#include <boost/thread.hpp>
#include <boost/date_time.hpp>

int (*functionPtr)(int, int);

void workerTask()
{
    boost::posix_time::seconds workTime(3);
    std::cout << "Task: running" << std::endl;

    boost::this_thread::sleep(workTime);
    if (functionPtr != NULL)
        std::cout << "Sum of 2 and 3:" 
            << functionPtr(2, 3) << std::endl;
    std::cout << "Task: finished" << std::endl;
}

void RegisterFuncPtr(int (*func)(int, int))
{
    functionPtr = func;
}