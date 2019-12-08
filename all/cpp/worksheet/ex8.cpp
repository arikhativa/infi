
#include <iostream>

using namespace std;

namespace nsa
{
    void Foo(){std:cout << "-- in nsa Foo()" << endl;}
    namespace nsa_a
    {
        void Foo(){std:cout << "-- in nsa_a Foo()" << endl;}
    }//namespace nsa_a
}//namespace nsa

namespace nsb
{
    void Foo(){std:cout << "-- in b Foo()" << endl;}
    void Bar(){std:cout << "-- in b Bar()" << endl;}
    void Fishi(){std:cout << "-- in b Fishi()" << endl;}
}//namespace nsb

namespace nsa
{
    void AFunc(){std:cout << "-- in a AFunc()" << endl;}
}

void A()
{
    nsa::Foo();
    using nsb::Foo;
    Foo();
}

//using namespace nsa;
using namespace nsb;
namespace nsaaa=nsa::nsa_a;

void B()
{
    nsaaa::Foo();
    Foo();
    Bar();
}

int main()
{
    //A();

    B();

    return 0;
}
