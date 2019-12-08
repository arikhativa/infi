
#include <iostream>

struct X
{
    X(int a, int b);

    void Inc();
    void Dec();
    void Print();

    int m_a;
    int m_b;
};

X::X(int a, int b): m_a(a), m_b(b)
{}

void X::Inc()
{
    ++m_a;
    ++m_b;
}

void X::Dec()
{
    --m_a;
    --m_b;
}

void X::Print()
{
    std::cout << "\tm_a - " << m_a;
    std::cout << "\tm_b - " << m_b;
    std::cout << std::endl;
}

int main()
{
    X x1(7, 9);

    std::cout << "\tsizezof X = " << sizeof(x1);
    std::cout << std::endl;

    x1.Print();
    x1.Inc();
    x1.Print();

    return 0;
}
