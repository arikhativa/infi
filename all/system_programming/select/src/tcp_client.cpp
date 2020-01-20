
#include <sys/types.h>      // socket()
#include <sys/socket.h>     // socket()
#include <string.h>         // memset()
#include <stddef.h>         // size_t
#include <netdb.h>          // struct sockaddr_in()
#include <stdio.h>          // printf()
#include <stdlib.h>           // exit()
#include <unistd.h>         // close()

#include "tcp.h"
#include "tcp_client.hpp"


namespace hrd11
{

TCPClient::TCPClient() : m_sockfd(TCPSocket())
{
    TCPInitAddr(&m_serv);
}

TCPClient::~TCPClient()
{
    close(m_sockfd);
}

void TCPClient::Connect()
{
    TCPConnect(m_sockfd, &m_serv);
}

void TCPClient::Send()
{
    char msg[] = "From TCP";

    TCPSend(m_sockfd, msg);
}

void TCPClient::Recv()
{
    TCPRecv(m_sockfd);
}

} // hrd11
