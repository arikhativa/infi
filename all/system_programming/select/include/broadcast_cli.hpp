

#ifndef __UDP_BROADCAST_CLI_H__
#define __UDP_BROADCAST_CLI_H__

#include <cstddef>         // ssize_t
#include <netdb.h>          // struct sockaddr_in()

#include "udp.h"

#define BUFF_SIZE   1024
#define PORT        8420

namespace hrd11
{

struct BroadcastClient
{
    BroadcastClient();
    ~BroadcastClient();

    void SendRecv();

    int m_sockfd;
    struct sockaddr_in m_serv;
};

} // hrd11

#endif  /* __UDP_BROADCAST_CLI_H__ */
