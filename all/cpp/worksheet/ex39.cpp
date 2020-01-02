
#include <iostream>
#include <typeinfo>


using namespace std;

struct X
{
	X()
	{
		cerr << "-- X defualt Ctor()\n";
		throw bad_cast();
	}
	X(int)
	{
		cerr << "-- X Ctor(int)\n";
	}
	~X()
	{
		cerr << "-- X Dtor\n";
	}
};

struct Y
{
	Y()
	{
		cerr << "-- Y Ctor\n";
	}
	~Y()
	{
		cerr << "-- Y Dtor\n";
	}
};

class L
{
public:
	L() : m_x(2)
	{
		cerr << "-- L Ctor\n";
	}
	~L()
	{
		cerr << "-- L Dtor\n";
	}
private:
	Y m_y;
	X m_x;
};


class M
{
public:
	M()
	{
		cerr << "-- M Ctor\n";
	}
	~M()
	{
		cerr << "-- M Dtor\n";
	}
private:
	X m_x;
	Y m_y;

};

class N
{
public:
	N()
	{
		cerr << "-- N Ctor\n";
	}
	~N()
	{
		cerr << "-- N Dtor\n";
	}
private:
	Y m_y;
	X m_x;

};

class J
{
public:
	J() : m_y(new Y), m_x(new X)
	{
		cerr << "-- J Ctor\n";
	}
	~J()
	{
		cerr << "-- J Dtor\n";
		delete m_y;
		delete m_x;
	}
private:
	Y* m_y;
	X* m_x;

};

class K
{
public:
	K()
	{
		cerr << "-- K Ctor\n";
		m_y = new Y;
		m_x = new X;
	}
	~K()
	{
		cerr << "-- K Dtor\n";
		delete m_x;
		delete m_y;
	}
private:
	Y* m_y;
	X* m_x;

};

void Foo()
{
		K var1;

}

int main()
{
	try
	{
		Foo();
	}
	// catch (K& e)
	// {
	// 	cerr << "-- exception cout. what " << e.what() << endl;
	// }
	catch (exception& e)
	{
		cerr << "-- exception cout. what " << e.what() << endl;
	}
	return 0;
}
