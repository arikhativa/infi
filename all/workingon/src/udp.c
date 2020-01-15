
#include <sys/types.h>      // socket()
#include <sys/socket.h>     // socket()
#include <string.h>         // memset()
#include <stddef.h>         // size_t
#include <netdb.h>          // struct sockaddr_in()
#include <stdio.h>          // printf()
#include <stdlib.h>           // exit()
#include <unistd.h>         // close()

#include "udp.h"

#define PORT_BROADCAST 7777
#define BUFF_SIZE   1024
#define PORT        8420

int Socket()
{
    int ret = socket(AF_INET, SOCK_DGRAM, 0);

    if (0 > ret)
    {
        printf("bad socket UDP\n\n");
        exit(0);
    }

    return ret;
}

int SocketBroadcast()
{
    int ret = socket(AF_INET, SOCK_DGRAM, 0);
    int broadcast = 1;

    if (0 > ret)
    {
        printf("bad socket BCAST\n\n");
        exit(0);
    }

    setsockopt(ret, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

    return ret;
}

void InitAddr(struct sockaddr_in *addr)
{
    memset(addr, 0, sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(PORT);
    addr->sin_addr.s_addr = INADDR_ANY;
}

void InitBroadcast(struct sockaddr_in *addr)
{
    memset(addr, 0, sizeof(struct sockaddr_in));
    addr->sin_family = PF_INET;
    addr->sin_port = htons(PORT_BROADCAST);
    addr->sin_addr.s_addr = INADDR_BROADCAST;
}

ssize_t Send(int *sockfd, char *msg, struct sockaddr_in *addr, socklen_t len)
{
    ssize_t ret = sendto(*sockfd, msg, strlen(msg) + 1, 0,
                        (struct sockaddr *)addr, len);
    if (0 > ret)
    {
        printf("bad send UPC\n\n");
        exit(0);
    }

    return ret;
}

ssize_t Recv(int *sockfd, struct sockaddr_in *addr, socklen_t *len)
{
    ssize_t n = 0;
    char buff[BUFF_SIZE];

    n = recvfrom(*sockfd, buff, BUFF_SIZE, 0, (struct sockaddr *)addr, len);
    buff[n] = '\0';
    printf("Recv:\t '%s'\n", buff);
}

int Bind(int *sockfd, struct sockaddr_in *addr)
{
    int ret = bind(*sockfd, (struct sockaddr *)addr, sizeof(struct sockaddr));

    if (ret)
    {
        printf("bad bind UPC\n\n");
        exit(0);
    }

    return ret;
}
