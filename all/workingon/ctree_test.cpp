
#include <sstream>

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>  // istream, ostream

#include "dir_handle.hpp"
#include "ctree.hpp"
#include "color.h"

using namespace hrd11;
using namespace std;

void TestBasicCycel(const std::string& path)
{
    std::shared_ptr<const Directory> dir = Directory::CreateInstance(path);

    if (dir)
    {
        dir->Display(cout, 1);
    }
}

void TestFind(const std::string& path1, const std::string& path2)
{
    std::shared_ptr<const Directory> dir1 = Directory::CreateInstance(path1);
    std::shared_ptr<const Directory> dir2 = dir1->Find(path2);

    if (dir2)
    {
        dir2->Display(cout, 1);
    }
}

int main(int ac, char **av)
{

    if (ac == 1)
    {
        TestBasicCycel(".");
    }
    if (ac == 2)
    {
        TestBasicCycel(av[1]);
    }
    if (ac == 3)
    {
        TestFind(av[1], av[2]);
    }


	return 0;
}
