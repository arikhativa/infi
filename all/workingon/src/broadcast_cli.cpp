
#include <sys/types.h>      // socket()
#include <sys/socket.h>     // socket()
#include <string.h>         // memset()
#include <stddef.h>         // size_t
#include <netdb.h>          // struct sockaddr_in()
#include <stdio.h>          // printf()
#include <stdlib.h>           // exit()
#include <unistd.h>         // close()

#include "udp.h"
#include "broadcast_cli.hpp"

namespace hrd11
{

BroadcastClient::BroadcastClient() : m_sockfd(SocketBroadcast())
{
    InitBroadcast(&m_serv);
}

BroadcastClient::~BroadcastClient()
{
    close(m_sockfd);
}

void BroadcastClient::SendRecv()
{
    socklen_t len = sizeof(struct sockaddr_in);
    char msg[] = "From Broadcast";

    Send(&m_sockfd, msg, &m_serv, len);
    Recv(&m_sockfd, &m_serv, &len);
}

} // hrd11
