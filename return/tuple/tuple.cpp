#include <iostream>
#include "tuple_helper.h"

std::tuple<bool, bool> GetStates();

int main()
{
    auto [ret1, ret2] = GetStates();
    std::cout << ret1 << ":" << ret2 << std::endl;

    std::tuple<bool,bool> a(true, false);
    std::tuple<bool,bool> b(false, true);
    a &= b;    
    std::cout << std::get<0>(a) << ":" << std::get<1>(a) << std::endl;


   std::tuple<int,double> t = std::make_tuple(1,2);
   auto t1 = t + t;
   std::cout << std::get<0>(t1) << std::endl;
   std::cout << std::get<1>(t1) << std::endl;
    return 0;
}

std::tuple<bool, bool> GetStates()
{
    return {true, false};
}