
#include <iostream>

class X
{
public:
    explicit X(int n): m_num(n){}
    //X(int n): m_num(n){}
    operator int () const
    {
        return m_num;
    }
    int GetNum() const
    {
        return m_num;
    }
private:
    int m_num;
    int m_num1;
    int m_num2;
};

void Foo(X x)
{
    std::cout << "-- x - "<< x << std::endl;
    std::cout << "-- x.GetNum - " << x.GetNum() << std::endl;
}

void Bar(int num)
{
    std::cout << "-- num - "<< num << std::endl;
}


int main()
{
    X x1(8);

    Foo(2);
    Bar(x1);


    return 0;
}
