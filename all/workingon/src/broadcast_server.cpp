
#include <sys/types.h>      // socket()
#include <sys/socket.h>     // socket()
#include <string.h>         // memset()
#include <stddef.h>         // size_t
#include <netdb.h>          // struct sockaddr_in()
#include <stdio.h>          // printf()
#include <stdlib.h>           // exit()
#include <unistd.h>         // close()

#include "udp.h"
#include "broadcast_server.hpp"

namespace hrd11
{

BroadcastServer::BroadcastServer() : m_sockfd(SocketBroadcast())
{
    InitBroadcast(&m_serv);
    Bind(&m_sockfd, &m_serv);
}

BroadcastServer::~BroadcastServer()
{
    close(m_sockfd);
}

void BroadcastServer::RecvSend()
{
    socklen_t len = sizeof(struct sockaddr_in);
    char msg[] = "From Server";

    Recv(&m_sockfd, &m_serv, &len);
    Send(&m_sockfd, msg, &m_serv, len);
}

} // hrd11
