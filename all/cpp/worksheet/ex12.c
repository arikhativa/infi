
#include <iostream>


int main()
{
    const int i = 3;
    //int *ip = (int*)&i;
    std::cout << &i;

    //*ip = 5;

    //std::cout << "-- &i\t" << &i << std::endl << "-- ip\t" << ip << std::endl;
    //std::cout << "-- i\t" << i << std::endl << "-- *ip\t" << *ip << std::endl;

    /*printf("-- &i\t%p\n-- ip\t%p\n", &i, ip);
    printf("-- i\t%d\n-- *ip\t%d\n", i, *ip);
    printf("-- i2\t%d\n-- *ip2\t%d\n", (int)i, *ip);*/

    return 0;
}
