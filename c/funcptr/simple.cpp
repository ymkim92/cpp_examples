#include <iostream>

int (*MulFunc)(int a, int b);

void RegisterMulFuncPtr(int (*callback)(int, int))
{
    MulFunc = callback;
}

int mul(int a, int b)
{
    return a*b;
}

int main()
{
    RegisterMulFuncPtr(mul);

    if (MulFunc != NULL)
        std::cout << "Mul of 2 and 3:" 
            << MulFunc(2, 3) << std::endl;

    return 0;
}
