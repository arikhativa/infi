
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>

#define BUFF 100
#define PORT "4200"
#define HOST "0108"


void *GetAddr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int ac, char** av)
{
    int sockfd = 0;
    int numbytes = 0;
    int rv = 0;
    char buf[BUFF];
    char s[INET6_ADDRSTRLEN];
    struct addrinfo hints;
    struct addrinfo* servinfo;
    struct addrinfo* p;
    char msg[BUFF] = "Pong";


    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    rv = getaddrinfo(HOST, PORT, &hints, &servinfo);

    p = servinfo;
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

    if (connect(sockfd, p->ai_addr, p->ai_addrlen))
    {
        printf("-- client: connect didnt worjk\n");
        return 1;
    }

    inet_ntop(p->ai_family, GetAddr((struct sockaddr *)p->ai_addr), s,sizeof(s));

    freeaddrinfo(servinfo);

    while (1)
    {
        numbytes = recv(sockfd, buf, BUFF-1,0 );
        buf[numbytes] = '\0';

        printf("-- client: %s\n", buf);

        send(sockfd, msg, sizeof(msg), 0);
    }


    close(sockfd);


    return 0;
}
