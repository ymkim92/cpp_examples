#include <iostream>
#include <tuple>

std::tuple<bool, bool> GetStates();

int main()
{
    auto [ret1, ret2] = GetStates();
    std::cout << ret1 << ":" << ret2 << std::endl;
    return 0;
}

std::tuple<bool, bool> GetStates()
{
    return {true, false};
}