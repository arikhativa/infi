
#include <iostream>

struct X
{
    explicit X(int a);
    ~X();

    void Foo();
    void Print() const;

    int m_a;
    int *m_p;
};

X::X(int a): m_a(a), m_p(new int(a))
{}

X::~X()
{
    delete m_p;
    m_p = NULL;
}

void X::Foo()
{
    ++m_a;
    --*m_p;
}

void Fifi(const X& x1)
{
    x1.Print();
}

//void X::Print()
void X::Print() const
{
    std::cout << "\tm_a - " << m_a;
    std::cout << std::endl;
    std::cout << "\tm_p - " << m_p;
    std::cout << std::endl;
    std::cout << "\t*m_p - " << *m_p;
    std::cout << std::endl;
    std::cout << std::endl;

    //m_a = 0;
    //m_p = 0;
    *m_p = 0;
}

int main()
{
    X x1;

    /*x1.Foo();
    Fifi(x1);*/

    return 0;
}
