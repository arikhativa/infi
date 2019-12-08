
#include <iostream>

static int g_tmp;

class X
{
public:

    X(): m_id(++s_i){}

    int GetId()
    {
        std::cout << m_id << std::endl;
        return m_id;
    }
    static void Foo()
    {
        s_i = 0;
    }

private:
    static int s_i;
    int m_id;
};


int main()
{
    X x1;
    X x2;


    X::Foo();

    // x1.GetId();
    // x2.GetId();

    return 0;
}
int X::s_i = 0;
