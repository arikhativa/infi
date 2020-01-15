
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
#define PORT 8080

int main()
{
    char msg[] = "Yoav";
    int sockfd = 0;
    struct sockaddr_in servaddr = {0};
    struct sockaddr_in cliaddr = {0};
    int n = 0;
    int len = 0;
    int stt = 0;
    char buff[BUFF] = {0};
    int broadcast = 1;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > sockfd)
    {
	    printf("-- Srver: error creating socket");
    }
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

    servaddr.sin_family = PF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    // inet_aton("10.3.0.46", &(servaddr.sin_addr));

    stt = bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (0 > stt)
    {
	    printf("-- Srver: error binding socket");
    }

    printf("server: waiting for connections...\n\n");

    while (1)
    {
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, buff, BUFF, 0, (struct sockaddr *)&cliaddr, &len);
        buff[n] = '\0';
        printf("server: recv this : '%s'\n", buff);
        sendto(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);

    return 0;
}
