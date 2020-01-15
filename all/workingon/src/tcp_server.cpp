
#include <sys/types.h>      // socket()
#include <sys/socket.h>     // socket()
#include <string.h>         // memset()
#include <stddef.h>         // size_t
#include <netdb.h>          // struct sockaddr_in()
#include <stdio.h>          // printf()
#include <stdlib.h>           // exit()
#include <unistd.h>         // close()

#include "tcp.h"
#include "tcp_server.hpp"


namespace hrd11
{

TCPServer::TCPServer() : m_sockfd(TCPSocket())
{
    TCPInitAddr(&m_serv);
    BindAndListen(&m_sockfd, &m_serv);
}

TCPServer::~TCPServer()
{
    close(m_sockfd);
}

void TCPServer::AcceptAndSend()
{
    Accept(m_sockfd);
}

} // hrd11
