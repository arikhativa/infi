
#include <sys/select.h>     /*  select() */
#include <unistd.h>     /*  close() */
#include <stdio.h>

#include "tcp_client.hpp"

using namespace hrd11;

int main()
{
    TCPClient tcp;

    tcp.Connect();
    tcp.Send();
    tcp.Recv();

    return 0;
}
