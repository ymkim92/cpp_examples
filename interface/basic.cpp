#include <cstdio>
struct Logger
{
    // Adding the virtual destructor causes the DerivedClass destructor to
    // get invoked when you delete the BaseClass pointer 
    virtual ~Logger() = default;

    // The =0 suffix specifies a pure virtual method
    // where a virtual function that is required to be implemented 
    // by a derived class if the derived class is not abstract (base class)

    // An abstract class is a class that is designed to be specifically used as a base class.
    virtual void log_transfer(long from, long to, double amount) = 0;
};

struct ConsoleLogger : Logger
{
    void log_transfer(long from, long to, double amount) override
    {
        printf("[cons] %ld -> %ld: %f\n", from, to, amount);
    }
};