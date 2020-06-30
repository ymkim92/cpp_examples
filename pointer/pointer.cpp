#include <iostream>
#include <typeinfo>
 
int main()
{
	int x{ 4 };
	std::cout << typeid(x).name() << '\n';
	std::cout << typeid(&x).name() << '\n'; // Pi in g++: pointer of int
	// std::cout << typeid(*x).name() << '\n';  // compilation error
 
	return 0;
}