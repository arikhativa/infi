
#include <sys/select.h>     /*  select() */
#include <unistd.h>     /*  close() */
#include <stdio.h>

#include "udp_server.hpp"
#include "tcp_server.hpp"
#include "broadcast_server.hpp"

#define STDIN 0

using namespace hrd11;

int max(int x, int y)
{
	if (x > y)
		return x;
	else
		return y;
}

int main()
{
	BroadcastServer bcast;
	UDPServer udp;
	TCPServer tcp;
    int fds = 1;
    fd_set rset;

    FD_ZERO(&rset);

	fds = max(fds - 1, udp.m_sockfd) + 1;
	fds = max(fds - 1, tcp.m_sockfd) + 1;
	fds = max(fds - 1, bcast.m_sockfd) + 1;
	fds = max(fds - 1, STDIN) + 1;

	int i = 0;
	while (i < 3)
	{
		FD_SET(udp.m_sockfd, &rset);
		FD_SET(tcp.m_sockfd, &rset);
		FD_SET(bcast.m_sockfd, &rset);
		FD_SET(STDIN, &rset);

		select(fds, &rset, NULL, NULL, NULL);

	    if (FD_ISSET(udp.m_sockfd, &rset))
	    {
			printf("-- in UDP\n");
	        udp.RecvSend();
	    }
	    if (FD_ISSET(tcp.m_sockfd, &rset))
	    {
			printf("-- in TCP\n");
	        tcp.AcceptAndSend();
	    }
	    if (FD_ISSET(bcast.m_sockfd, &rset))
	    {
			printf("-- in Broadcast\n");
			bcast.RecvSend();
	    }
	    if (FD_ISSET(STDIN, &rset))
	    {
			char msg[BUFF_SIZE] = {0};
			read(STDIN, msg, BUFF_SIZE -1);

			printf("-- in sdin\n");
			printf("-- %s\n", msg);
	    }

		++i;
	}

    return 0;
}
