

#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include <cstddef>         // ssize_t
#include <netdb.h>          // struct sockaddr_in()

#include "tcp.h"

#define BUFF_SIZE   1024
#define PORT        8420

namespace hrd11
{

struct TCPServer
{
    TCPServer();
    ~TCPServer();

    void AcceptAndSend();
    // int SSS();


    int m_sockfd;
    struct sockaddr_in m_serv;
};

} // hrd11

#endif  /* __TCP_SERVER_H__ */
