
#include <sys/select.h>     /*  select() */
#include <unistd.h>     /*  close() */
#include <stdio.h>

#include "udp_client.hpp"

using namespace hrd11;

int main()
{
    UDPClient udp;

    udp.SendRecv();

    return 0;
}
