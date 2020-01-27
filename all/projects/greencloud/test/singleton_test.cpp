
#include <unistd.h>

#include <thread>			//std::thread

#include <dlfcn.h>			/* dlopen(), dlclose, dlsym */


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


static void ThreadFunc()
{
    for (int i = 0; i < 1000000; ++i)
    {
        Base* ptr = Singleton<Base>::GetInstance();
        (void)ptr;
    }

}

static void ExplineTest()
{
    printf("-- Test For Singleton\n");
    printf("-- Description:\n");
    printf("-- \t%s\n", "class Tmp has a static int m_x");
    printf("-- \t%s\n", "Every time the ctor is called");
    printf("-- \t%s\n", "it dose ++m_x");
    printf("-- \t%s\n", "");
}

static void ThreadTest()
{
    std::thread th1(ThreadFunc);
    std::thread th2(ThreadFunc);
    std::thread th3(ThreadFunc);
    std::thread th4(ThreadFunc);
    std::thread th5(ThreadFunc);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
}

typedef void (*fp_t)();

static void RunTimeLinkTest()
{
    Base* ptr = Singleton<Base>::GetInstance();

    void *handle = 0;
    fp_t func = 0;

    handle = dlopen("/usr/lib/libFoo.so", RTLD_LAZY);
    if (!handle)
    {
        printf("-- can't find lib\n");
        exit(1);
    }
    func = (fp_t)dlsym(handle, "Foo");
    if (!func)
    {
        printf("-- can't open lib\n");
        printf("%s\n", dlerror());
        exit(1);
    }

    printf("-- Main()\n");
    printf("-- Base ptr1 %p\n", (void*)ptr);
    Base* ptr1 = Singleton<Base>::GetInstance();
    printf("-- Base ptr2 %p\n", (void*)ptr1);

    func();
}

int main()
{
    Base* ptr = Singleton<Base>::GetInstance();

    ThreadTest();
    RunTimeLinkTest();

    if ('1' != ptr->m_x)
    {
        ExplineTest();
        printf("-- Fail...\n");
        printf("-- The Ctor was called %c times\n\n", ptr->m_x);
    }
    else
    {
        printf("-- Success :)\n");
    }



    return 0;
}
