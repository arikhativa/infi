
#include <iostream>
#include <cstdlib>

using namespace std;

//struct bad: public runtime_error
struct bad
{
    //bad(const string& s = "badbad\n") : runtime_error(s)
    bad(const string& s = "badbad\n") : m_s(s)
    {
	    std::cout << "-- Ctor" << m_s << "\n\n";
    }
    ~bad()
    {
	    std::cout << "-- Dtor" << m_s << "\n\n";
    }
    const string m_s;
};

void Fifi() throw(bad)
{
    //throw bad("bad2\n\n");
    cerr << "Fifi before throw\n";
//    throw int(7);
    throw bad("1212");     // throws int (in exception-specification)
    cerr << "Fifi after throw\n";
}

void Foo()
{
	bad("66");
    Fifi();
    cerr << "Foo() after Fifi()\n\n";
}

void Bar()
{
    Foo();
    cerr << "Bar() after Foo()\n\n";
}

void myterminate() throw();

void myunexpected()
{
	  std::cerr << "unexpected called\n";
	 throw bad("777");     // throws int (in exception-specification)
}

int main()
{
	std::set_terminate (myterminate);

	std::set_unexpected (myunexpected);
	bad t1("1");
	bad t2("2");
   try
    {
	bad t3("3");
        Fifi();
        //throw bad("5");
    }
    catch(bad_alloc)
    {
        cerr << "-- no mem\n";
        exit(2);
    }
    catch(bad& b)
    {
        throw int(0);
        cerr << "bad exception - ";
//        exit(3);
    }
    catch( exception& r )
    {
        // exit(4);
        //cerr << "-- unknoew exception : \n" << r.what();
    }
    cerr << "\n\n-- no catch\n\n";
    return 0;
}

void myterminate() throw()
{
	  std::cerr << "terminate handler called\n";
	   abort();  // forces abnormal termination
}
