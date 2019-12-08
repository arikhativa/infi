
#include <iostream>

struct X
{
    explicit X();
    explicit X(int a_, int b_ = 8);
    ~X();
    X(const X& other_);
    X &operator=(const X& other_);

    int m_a;
    int m_b;
};

X::X(): m_a(3), m_b(4)
{
    std::cout << "in X()" << std::endl;
    std::cout << "'this'\t" << this << std::endl;
    std::cout << "m_a\t" << m_a << "\nm_b\t" << m_b << std::endl << std::endl;
}

X::X(int a_, int b_): m_a(a_), m_b(b_)
{
    std::cout << "in a_ b_" << std::endl;
    std::cout << "'this'\t" << this << std::endl;
    std::cout << "m_a\t" << m_a << "\nm_b\t" << m_b << std::endl << std::endl;
}

X::X(const X& other_): m_a(other_.m_a), m_b(other_.m_b)
{
    std::cout << "in const other_" << std::endl;
    std::cout << "'this'\t" << this << std::endl;
    std::cout << "m_a\t" << m_a << "\nm_b\t" << m_b << std::endl << std::endl;
}

X& X::operator=(const X& other_)
{
    m_a = other_.m_a;
    m_b = other_.m_b;

    std::cout << "in oper=" << std::endl;
    std::cout << "'this'\t" << this << std::endl;
    std::cout << "m_a\t" << m_a << "\nm_b\t" << m_b << std::endl << std::endl;

    return *this;
}

X::~X()
{
    std::cout << "in ~X" << std::endl;
    std::cout << "'this'\t" << this << std::endl;
    std::cout << "m_a\t" << m_a << "\nm_b\t" << m_b << std::endl << std::endl;
}

int main()
{
    X x1;
    X x2(7);
    X x3(9, 10);
    X x4(x1);
    X *px = new X(x2);
    X* ptr_x = new X[10];

    delete ptr_x;

    x1 = x3;

    delete px;
    px = 0;

    std::cout << "sizeof\t" << sizeof(X) << std::endl;


    return 0;
}
