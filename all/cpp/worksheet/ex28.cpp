
#include <iostream>

class X
{
public:
    X() : m_a(5)
    {
        std::cout << "-- Ctor X - " << this;
        std::cout << std::endl;

    }
    virtual ~X()
    {
        std::cout << "-- Dtor X - " << this;
        std::cout << std::endl;
    }
    void Foo(X x1)
    {
        x1.m_a = 32;
    }

protected:

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
    int m_b;
private:
};

int main()
{
    //X* xp = new Y[5];
    //X* xp = new Y;
    //x1.m_a = 6;
    //Y* tmp = static_cast<Y*>(xp);

    // std::cout << std::endl;
    // std::cout << "-- sizeof(Y) - " << sizeof(Y);
    // std::cout << std::endl;
    // std::cout << "-- sizeof(X) - " << sizeof(X);
    // std::cout << std::endl;
     // std::cout << "-- x.m_a - " << xp->m_a;
     // std::cout << std::endl;
     //
     //
     // //delete[] tmp;
     // // delete[] xp;
     //  delete xp;

    return 0;
}
