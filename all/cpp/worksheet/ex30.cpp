
#include <iostream>

class X
{
public:
    X(double d) : m_a(d) {}
    virtual ~X(){}
    void Print()
    {
        std::cout << "-- Print X  m_b - " << m_a;
        std::cout << std::endl;
    }
private:
    double m_a;
};

class Y : public X
{
public:
    Y(double d, int i) : X(d), m_b(i) {}
    void Print()
    {
        std::cout << "-- Print Y  m_b - " << m_b;
        std::cout << std::endl;
    }
private:
    int m_b;
};
// Y y1(0.0, 0);
// Y y2(14.2, 5);
// Y y4(14.2, 5);


// X& x1 = y1;
// X& x2 = y2;

// x1 = x2;
// y3 = y4;

int main()
{
    Y y3(0.0, 0);
    X* p = &y3;

    p->Print();
}
