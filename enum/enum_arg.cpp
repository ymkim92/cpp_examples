#include <iostream>

class Test {
public:
    enum Arg {
        ARG1,
        ARG2
    };
public:
    static void foo(const Arg& a);
};

void Test::foo(const Arg& a)
{
    std::cout << a << std::endl;
}

int main()
{
    Test::foo(Test::ARG2);
    Test::foo(static_cast<Test::Arg>(0));
    Test::foo(static_cast<Test::Arg>(3));
}