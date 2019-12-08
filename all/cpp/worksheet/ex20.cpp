
#include <iostream>

struct X
{
    //explicit X();
//    explicit X(int a_, int b_ = 8);
//    ~X();
    //X(const X& other_);
    //X &operator=(const X& other_);

    int m_a;
    int m_b;
};

struct Y
{
//    explicit Y();
    X m_x;
//    int m_i;
};

/*Y::Y(): m_i(4), m_x(3, 3)
{
    std::cout << "in Y()" << std::endl;
    std::cout << "'this'\t" << this << std::endl;
}*/
/*X::X(): m_a(3), m_b(4)
{
    std::cout << "in X()" << std::endl;
    std::cout << "'this'\t" << this << std::endl;
    std::cout << "m_a\t" << m_a << "\nm_b\t" << m_b << std::endl << std::endl;
}*/

/*X::X(int a_, int b_): m_a(a_), m_b(b_)
{
    std::cout << "in a_ b_" << std::endl;
    std::cout << "'this'\t" << this << std::endl;
    std::cout << "m_a\t" << m_a << "\nm_b\t" << m_b << std::endl << std::endl;
}*/

/*X::X(const X& other_): m_a(other_.m_a), m_b(other_.m_b)
{
    std::cout << "in const other_" << std::endl;
    std::cout << "'this'\t" << this << std::endl;
    std::cout << "m_a\t" << m_a << "\nm_b\t" << m_b << std::endl << std::endl;
}*/

/*X& X::operator=(const X& other_)
{
    m_a = other_.m_a;
    m_b = other_.m_b;

    std::cout << "in oper=" << std::endl;
    std::cout << "'this'\t" << this << std::endl;
    std::cout << "m_a\t" << m_a << "\nm_b\t" << m_b << std::endl << std::endl;

    return *this;
}*/

/*X::~X()
{
    std::cout << "in ~X" << std::endl;
    std::cout << "'this'\t" << this << std::endl;
    std::cout << "m_a\t" << m_a << "\nm_b\t" << m_b << std::endl << std::endl;
}*/

int main()
{
    Y y1;
    y1.m_x.m_a = 9;
    /*Y y2(y1);
    Y y3;

    y1.m_x.m_a = 250;
    y1.m_x.m_b = 100;

    y3 = y1;*/

    return 0;
}
