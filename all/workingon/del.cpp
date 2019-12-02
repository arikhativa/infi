#include <iostream>
using namespace std;

namespace ns
{
    // Only declaring class here
    class geek;
}

// Defining class outside
class ns::geek
{
public:
    void display()
    {
        cout << "ns::geek::display()\n";
    }
};

int main()
{
    //Creating Object of geek Class
    geek obj;
    obj.display();
    return 0;
}
