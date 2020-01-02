
#include <iostream>
#include <typeinfo>

using namespace std;

int Foo(int i) throw(bad_alloc)
{
	i += 7;

	cout << "-- in Foo \n";
	throw(bad_alloc());

	return i;
}

void Bar() throw(bad_cast)
{
	cout << "-- in Bar \n";
	throw(bad_cast());
}

class X
{
public:
	X() : m_a(7)
	{
		cout << "-- in X Ctor \n";
	}
	X(const X& x)
	{
		cout << "-- in X CCtor \n";
	}
	~X()
	{
		cout << "-- in X Dtor \n";
		// cerr << Foo(m_a) << endl;
	}
private:
	int m_a;
};

void Fishi()
{
	cout << "-- in Fishi \n";
	X x1;

	Bar();
}

void func1()
{
	cout << "-- in unexpected \n";
	throw int (10);
}

int main()
{
	// set_unexpected (func1);
int i;
	cout << "-- &stack	-	" << &i<< endl;
	try
	{
		// Fishi();
		throw X();
	}
	catch (X x)
	{

		cout << "-- in X\n";
		cout << "-- in &X	-	" << &x << endl;
	}
	catch (int)
	{
		cout << "-- in int\n";
	}
	catch (bad_alloc)
	{
		cout << "-- in bad_alloc\n";
	}
	catch (bad_cast)
	{
		cout << "-- in bad_cast\n";
	}

	cout << "-- end of main \n";
	return 0;
}
