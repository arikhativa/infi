
#include <iostream>


using namespace std;

class X;

// Global overload for '+' operator
//int operator+(const X& x1, const X& x2);


// Overload for '<<' operator in class 'ostream'
//ostream& operator<<(ostream &os, const X &x);

class X
{
    friend ostream &operator<<(ostream &o, const X &x);
public:
    int operator+(const X& x2)
    {
        return m_a + x2.m_a;
    }
    bool operator==(const X &o) const
    {
        return m_a==o.m_a;
    }
private:
    //friend int operator+(const X& x1, const X& x2);
    int m_a;
};

ostream &operator<<(ostream &os, const X &x)
{
    return os <<x.m_a;
}

X Foo()
{
    return X();
}

// template <typename type>
// void operator+(type a, type b)
// {
//     std::cout << "heyyy" << std::endl;
// }


int main()
{
    X x1;
    X x2;

    std::cout << "\tx1\t" << x1;
    std::cout << "\tx2\t" << x2 << std::endl;
    std::cout << "\t+\t" << (x1 + x2) << std::endl;
    std::cout << "\t==\t" << (x1 == x2) << std::endl;
    // std::cout << "\t+\t" << x1.operator+(x2) << std::endl;
    // std::cout << "\t+\t" << x1.operator==(x2) << std::endl;

    return 0;
}
