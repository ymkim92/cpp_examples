#include <iostream>

int main()
{
    int x{5};
    int &lref{x};
    int &&rref{5};
    // int &&rref2{x};   // compilation error
    int y{x};

    std::cout << lref << std::endl;
    std::cout << rref << std::endl;

    std::cout << &x << std::endl;
    std::cout << &lref << std::endl;
    std::cout << &rref << std::endl;
	// std::cout << typeid(lref).name() << '\n';
	// std::cout << typeid(rref).name() << '\n';
    return 0;
}