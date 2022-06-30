#include <stdio.h>
#include <stdint.h>

/**
 * @brief This function does something like rounding but not usual rounding.
 * If the interval is 0.05, soc of 0.89 will be 0.9 
 * 0.95 => 1.0
 * 0.0 => 0.05
 * 
 * @param soc should be 0.0 ~ 1.0
 * @param interval to make 5% interval, use 0.05
 * @return double 
 */
uint8_t ReducePrecision(const uint8_t& soc, const uint8_t& interval)
{    
    return (soc/interval+1) * interval;
}

int main()
{
    for (uint8_t i=0; i<=100; i++) {
        printf("%d: %d\n", i, ReducePrecision(i, 5));
    }
}