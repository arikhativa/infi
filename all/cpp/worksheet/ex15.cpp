
#include <iostream>

using namespace std;

template <typename T>
void Foo(T i)
{
    cout << i << "Gen Foo\n";
}

void Foo(int i)
{
    cout << i << "Special Foo\n";
}

int main()
{
    Foo<int>(6);
    Foo('6');
    return 0;
}
