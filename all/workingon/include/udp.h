

#ifndef __UDP_H__
#define __UDP_H__

#include <stddef.h>         // ssize_t
#include <netdb.h>          // struct sockaddr_in()

int Socket();
int SocketBroadcast();

void InitAddr(struct sockaddr_in *addr);
void InitBroadcast(struct sockaddr_in *addr);

int Bind(int *sockfd, struct sockaddr_in *addr);

ssize_t Recv(int *sockfd, struct sockaddr_in *addr, socklen_t *len);
ssize_t Send(int *sockfd, char *msg, struct sockaddr_in *addr, socklen_t len);

#endif  /* __UDP_H__ */
