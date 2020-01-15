

#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <cstddef>         // ssize_t
#include <netdb.h>          // struct sockaddr_in()

#include "tcp.h"

#define BUFF_SIZE   1024
#define PORT        8420

namespace hrd11
{

struct TCPClient
{
    TCPClient();
    ~TCPClient();

    void Connect();
    void Send();
    void Recv();

    int m_sockfd;
    struct sockaddr_in m_serv;
};

} // hrd11

#endif  /* __TCP_CLIENT_H__ */
