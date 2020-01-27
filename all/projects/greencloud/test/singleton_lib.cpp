
#include <unistd.h>

#include "singleton.hpp"

using namespace hrd11;

struct Base
{
public:
    ~Base() = default;
    static int m_x;
    Base()
    {
        ++m_x;
    }
private:
    template<typename T>
    friend class hrd11::Singleton;

};

int Base::m_x = '0';


extern "C"
{
    void Foo()
    {
        Base* ptr = Singleton<Base>::GetInstance();

        printf("-- Foo()\n");
        printf("-- Base ptr1 %p\n", (void*)ptr);
        Base* ptr1 = Singleton<Base>::GetInstance();
        printf("-- Base ptr2 %p\n", (void*)ptr1);
    }
}
