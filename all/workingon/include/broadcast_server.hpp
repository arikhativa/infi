

#ifndef __UDP_BROADCAST_SERVER_H__
#define __UDP_BROADCAST_SERVER_H__

#include <cstddef>         // ssize_t
#include <netdb.h>          // struct sockaddr_in()

#include "udp.h"

#define BUFF_SIZE   1024
#define PORT        8420

namespace hrd11
{

struct BroadcastServer
{
    BroadcastServer();
    ~BroadcastServer();

    void RecvSend();

    int m_sockfd;
    struct sockaddr_in m_serv;
    struct sockaddr_in m_client;
};

} // hrd11

#endif  /* __UDP_BROADCAST_SERVER_H__ */
