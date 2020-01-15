

#ifndef __TCP_H__
#define __TCP_H__

#include <stddef.h>         // ssize_t
#include <netdb.h>          // struct sockaddr_in()

#define BUFF_SIZE   1024
#define PORT        8420

int TCPSocket();

void TCPInitAddr(struct sockaddr_in *addr);

int BindAndListen(int *sockfd, struct sockaddr_in *addr);

void Accept(int sockfd);

void TCPSend(int sockfd, char *msg);

void TCPRecv(int sockfd);

void TCPConnect(int sockfd, struct sockaddr_in *addr);



#endif  /* __TCP_H__ */
