
#include <sys/select.h>     /*  select() */
#include <unistd.h>     /*  close() */
#include <stdio.h>

#include "broadcast_cli.hpp"

using namespace hrd11;

int main()
{
    BroadcastClient bcast;

    bcast.SendRecv();

    return 0;
}
