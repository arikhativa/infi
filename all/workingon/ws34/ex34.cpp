
#include <iostream>
#include <cstdlib>

using namespace std;

struct bad: public runtime_error
{
    bad(const string& s = "badbad\n") : runtime_error(s)
    {}
};

void Fifi()
{
    throw bad("bad2\n\n");
    cerr << "Fifi after throw\n";
}

void Foo()
{
    Fifi();
    cerr << "Foo() after Fifi()\n\n";
}

void Bar()
{
    Foo();
    // cerr << "Bar() after Foo()\n\n";
}

int main()
{
    try
    {
        throw exception();
        // Bar();
    }
    catch(bad_alloc)
    {
        cerr << "-- no mem\n";
        exit(2);
    }
    catch(bad& b)
    {
        cerr << "bad exception - " << b.what();
        exit(3);
    }
    catch( exception& r )
    {
        // exit(4);
        cerr << "-- unknoew exception : \n" << r.what();
    }
    cerr << "\n\n-- unknoew exception : 2352346\n\n";
    return 0;
}
