
#include <iostream>

#include "ex14.h"

int main()
{
    int arr[12];


    using namespace std;

    cout << arr << endl;
    cout << Max<int*>(arr, arr + 8) << endl;
    cout << Max<int*>(arr, arr + 8) << endl;
    int a = 0x0fffff00;
    int b = 0x00000001;

    cout << Max(6.6,3.3 ) << endl;

    return 0;
}
