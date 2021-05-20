#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include "task1.h"

int sum(int a)
{
    static int sum = 0;
    sum = sum + a;
    return sum;
}

void workerTask()
{
    std::cout << "Task: running" << std::endl;

    sum(2);

    std::cout << "Task: finished" << std::endl;
}
