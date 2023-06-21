#include <iostream>

typedef int (*functionPtr)(int, int);
// int (*MulFunc)(int a, int b);
functionPtr MulFunc;

void RegisterMulFuncPtr(functionPtr func)
{
    MulFunc = func;
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
