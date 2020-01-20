
#include <sys/types.h>      // socket()
#include <sys/socket.h>     // socket()
#include <string.h>         // memset()
#include <stddef.h>         // size_t
#include <netdb.h>          // struct sockaddr_in()
#include <stdio.h>          // printf()
#include <stdlib.h>           // exit()
#include <unistd.h>         // close()

#include "udp.h"
#include "udp_server.hpp"


namespace hrd11
{

UDPServer::UDPServer() : m_sockfd(Socket())
{
    InitAddr(&m_serv);
    Bind(&m_sockfd, &m_serv);
}

UDPServer::~UDPServer()
{
    close(m_sockfd);
}

void UDPServer::RecvSend()
{
    socklen_t len = sizeof(struct sockaddr_in);
    char msg[] = "From Server";

    Recv(&m_sockfd, &m_serv, &len);
    Send(&m_sockfd, msg, &m_serv, len);
}

} // hrd11
