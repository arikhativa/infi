
#include <iostream>

class B
{
public:
    B(int a) : m_a(a)
    {
        std::cout << "-- in B ctor\n";
    }
    virtual ~B()
    {
        std::cout << "-- in B dtor\n";
    }
    virtual void Print1() const
    {
        std::cout << "-- in B::P1 m_a is " << m_a << std::endl;
    }
    virtual void Print2() const
    {
        std::cout << "-- in B::P2 \n";
    }
    virtual void Print3() const
    {
        std::cout << "-- in B::P3 \n";
    }
private:
    int m_a;
};

class X: public B
{
public:
    X() :  B(0), m_b(1)
    {
        std::cout << "-- in X ctor\n";
    }
    ~X()
    {
        std::cout << "-- in X dtor\n";
    }
    void Print1() const
    {
        std::cout << "-- in X P1 m_a is " << m_b << std::endl;
        B::Print1();
        std::cout << "-- End X P1 " << std::endl;
    }
    void Print2() const
    {
        std::cout << "-- in X P2 m_a is " << std::endl;
    }
    void Print5() const
    {
        std::cout << "-- in X P5" << std::endl;
    }
private:
    int m_b;
};


void Foo();


int main()
{
    //B* b1 = new B(0);
    B* b2 = new X;

    // std::cout << "main b1 - \n";
    // b1->Print1();
    // b1->Print2();
    // b1->Print3();
    //
    // std::cout << "main b2 - \n";
    // b2->Print1();
    // b2->Print2();
    // b2->Print3();

    //X* xx = static_cast<X*>(b2);
    std::cout << "main xx - \n";
    //xx->Print2();
    b2->Print2();

    //delete b1;
    delete b2;

    return 0;
}
