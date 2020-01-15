
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
#define PORT 4200


int main()
{

    char msg[] = "-- Pong";
    int sockfd = 0;
    struct sockaddr_in servaddr = {0};
    struct sockaddr_in cliaddr = {0};
    int n = 0;
    int len = 0;
    int stt = 0;
    char buff[BUFF] = {0};


    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > sockfd)
    {
	    printf("-- Srver: error creating socket");
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    stt = bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (0 > stt)
    {
	    printf("-- Srver: error binding socket");
    }


    while (1)
    {
        printf("server: waiting for connections...\n\n");
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, buff, BUFF, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buff[n] = '\0';
        printf("server: recv this : '%s'\n", buff);
        sendto(sockfd, msg, sizeof(msg), MSG_CONFIRM, (struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);

    return 0;
}
