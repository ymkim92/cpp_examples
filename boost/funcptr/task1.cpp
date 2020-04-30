#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include "task1.h"

functionPtr funcSum;
functionPtr funcMul;

void workerTask()
{
    boost::posix_time::seconds workTime(2);
    std::cout << "Task: running" << std::endl;

    boost::this_thread::sleep(workTime);
    if (funcSum != NULL)
        std::cout << "Sum of 2 and 3:" 
            << funcSum(2, 3) << std::endl;
    if (funcMul != NULL)
        std::cout << "Mul of 2 and 3:" 
            << funcMul(2, 3) << std::endl;
    std::cout << "Task: finished" << std::endl;
}

void RegisterSumFuncPtr(functionPtr func)
{
    funcSum = func;
}

void RegisterMulFuncPtr(functionPtr func)
{
    funcMul = func;
}