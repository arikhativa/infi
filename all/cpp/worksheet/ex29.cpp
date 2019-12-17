


#include <iostream>

class X
{
public:
    X()
    {
        std::cout << "-- Ctor X - " << this;
        std::cout << std::endl;

    }
    virtual ~X()
    {
        std::cout << "-- Dtor X - " << this;
        std::cout << std::endl;
    }

    double m_a;
private:
};

class Y : public X
{
public:
    Y()
    {
        std::cout << "-- Ctor Y - " << this;
        std::cout << std::endl;
    }
    ~Y()
    {
        std::cout << "-- Dtor Y - " << this;
        std::cout << std::endl;
    }
    double m_b;
private:
};

void Foo(X x)
{
    std::cout << "-- Foo() m_a - " << x.m_a;
    std::cout << std::endl;
    x.m_a = 55;

    return ;
}

int main()
{
    Y y1;

    y1.m_a = 66;
    Foo(y1);
    std::cout << "-- y1 m_a - " << y1.m_a;

    return 0;
}
