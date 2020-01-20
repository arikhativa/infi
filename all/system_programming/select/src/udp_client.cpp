
#include <sys/types.h>      // socket()
#include <sys/socket.h>     // socket()
#include <string.h>         // memset()
#include <stddef.h>         // size_t
#include <netdb.h>          // struct sockaddr_in()
#include <stdio.h>          // printf()
#include <stdlib.h>           // exit()
#include <unistd.h>         // close()

#include "udp.h"
#include "udp_client.hpp"

namespace hrd11
{

UDPClient::UDPClient() : m_sockfd(Socket())
{
    InitAddr(&m_serv);
}

UDPClient::~UDPClient()
{
    close(m_sockfd);
}

void UDPClient::SendRecv()
{
    socklen_t len = sizeof(struct sockaddr_in);
    char msg[] = "From UDP";

    Send(&m_sockfd, msg, &m_serv, len);
    Recv(&m_sockfd, &m_serv, &len);
}

} // hrd11
