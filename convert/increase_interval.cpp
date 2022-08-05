#include <stdio.h>

/**
 * @brief This function does something like rounding but not usual rounding.
 * If the interval is 0.05, soc of 0.89 will be 0.9 
 * 0.96 => 1.0
 * 0.95 => 0.95
 * 0.0 => 0.05
 * 
 * @param soc should be 0.0 ~ 1.0
 * @param interval to make 5% interval, use 0.05
 * @return double 
 */
double ChangePrecision(const double& soc, const double& interval)
{    
    return ((static_cast<int>((soc-0.01)/interval)+1)) * interval;
}

int main()
{
    for (double i=0.0; i<=1.0; i+=0.01) {
        printf("%lf: %lf\n", i, ChangePrecision(i, 0.05));
    }
}