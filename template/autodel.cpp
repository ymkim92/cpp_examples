#include <iostream>

template <class T>
class Auto_ptr1
{
    T* m_ptr;
public:
    Auto_ptr1(T* ptr=nullptr)
        :m_ptr(ptr)
    {
    }

    ~Auto_ptr1()
    {
        delete m_ptr;
    }

    T& operator*() const {return *m_ptr;}
    T* operator->() const {return m_ptr;}
};

class Resource
{
public:
    Resource() {std::cout << "Resource acquired\n";}
    ~Resource() {std::cout << "Resource destroyed\n";}
};

void passByValue(Auto_ptr1<Resource> res)
{
}

int main()
{
    Auto_ptr1<Resource> res1(new Resource());
    passByValue(res1);
    // Auto_ptr1<Resource> res2(res1);

    return 0;
}