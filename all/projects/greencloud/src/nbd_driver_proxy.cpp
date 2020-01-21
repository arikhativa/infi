
// del this
#include <stdio.h>


#include <unistd.h>		// close()
#include <sys/types.h>		// socketpair(), open()
#include <sys/socket.h>		// socketpair()
#include <signal.h>		// sigfilset(), sigprocmask()
#include <sys/ioctl.h>		// ioctl()
#include <linux/nbd.h>		// NBD macros
#include <sys/stat.h>		// open()
#include <fcntl.h>		// open()

#include <string>		// std::string


#include "nbd_driver_proxy.hpp"

namespace hrd11
{

enum SocketName
{
	REQUEST = 0,
	NBD
};

static void SetSignalMask()
{
	sigset_t sigset;
	int stt = 0;

	stt = sigfillset(&sigset);
	stt += sigprocmask(SIG_SETMASK, &sigset, NULL);

	if (stt)
	{
		perror("NBDDriverProxy SetSignal() - fail\n");
	}
}

static void ThreadFuncSetNBD(int file_fd, int socket_fd)
{
	int stt = 0;

	SetSignalMask();

	stt = ioctl(file_fd, NBD_SET_SOCK, socket_fd);
	if (stt)
	{
		perror("NBDDriverProxy Ctor - ThreadFunc- ioctl(SET_SOCK) fail\n");
	}
	stt = ioctl(file_fd, NBD_DO_IT);
	if (stt)
	{
		perror("NBDDriverProxy Ctor - ThreadFunc- ioctl(DO_IT) fail\n");
	}
	stt = ioctl(file_fd, NBD_CLEAR_QUE);
	stt += ioctl(file_fd, NBD_CLEAR_SOCK);
	if (stt)
	{
		perror("NBDDriverProxy Ctor - ThreadFunc- ioctl(CLEAR) fail\n");
	}
}

void Disconnect_NBD(int i)
{
	(void)i;
}

static int SetSigaction(int sig, const struct sigaction* act)
{
	int ret = 0;
	struct sigaction oact;

	ret = sigaction(sig, act, &oact);

	if (0 == ret && oact.sa_handler != SIG_DFL)
	{
		perror("NBDDriverProxy Ctor - SetSigaction() fail\n");
	}

	return ret;
}

static void SetSignals()
{
	struct sigaction act;
	int stt = 0;

	act.sa_handler = Disconnect_NBD;
	act.sa_flags = SA_RESTART;

	stt = sigemptyset(&act.sa_mask);
	stt += sigaddset(&act.sa_mask, SIGINT);
	stt *= 100;
	stt += sigaddset(&act.sa_mask, SIGTERM);
	if (stt)
	{
		perror("NBDDriverProxy Ctor - SetSignals(1)  fail\n");
	}

	stt = SetSigaction(SIGINT, &act);
	stt += SetSigaction(SIGTERM, &act);
	if (stt)
	{
		perror("NBDDriverProxy Ctor - SetSignals(2) fail\n");
	}
}

static void InitNBD(size_t size, const std::string& device_name, int* m_req_fd, int* m_nbd_fd, int* m_file_fd, std::thread& thread)
{
	int socket_pair[2];
	int stt = 0;

	stt = socketpair(AF_UNIX, SOCK_STREAM, 0, socket_pair);
	if (stt)
	{
		perror("NBDDriverProxy Ctor - socketpair() fail\n");
	}
	*m_req_fd = socket_pair[REQUEST];
	*m_nbd_fd = socket_pair[NBD];

	*m_file_fd = open(device_name.c_str(), O_RDWR);
	if (0 > *m_file_fd)
	{
		perror("NBDDriverProxy Ctor - open() fail\n");
	}

	stt = ioctl(*m_file_fd, NBD_SET_SIZE, size);
	if (-1 == stt)
	{
		perror("NBDDriverProxy Ctor - ioctl(SET_SIZE) fail\n");
	}
	stt = ioctl(*m_file_fd, NBD_CLEAR_SOCK);
	if (-1 == stt)
	{
		perror("NBDDriverProxy Ctor - ioctl(CLEAR_SOCK) fail\n");
	}

	thread = std::thread(ThreadFuncSetNBD, *m_file_fd, *m_nbd_fd);

	SetSignals();
}

NBDDriverProxy::NBDDriverProxy(size_t storage_size, const std::string& device_name) :
	m_size(storage_size),
	m_device_name(device_name)
{
	InitNBD(m_size, device_name, &m_req_fd, &m_nbd_fd, &m_file_fd, m_nbd_thread);
}

NBDDriverProxy::NBDDriverProxy(size_t block_size, size_t num_blocks, const std::string& device_name) :
	m_size(block_size * num_blocks),
	m_device_name(device_name)

{
}

NBDDriverProxy::~NBDDriverProxy()
{
	// join thread?
	close(m_req_fd);
	close(m_nbd_fd);
}

std::unique_ptr<DriverData> NBDDriverProxy::ReceiveRequest()
{
}

void NBDDriverProxy::SendReply(std::unique_ptr<DriverData> data)
{
	(void)data;
}

void NBDDriverProxy::Disconnect()
{
}

int NBDDriverProxy::GetReqFd()
{
	return m_req_fd;
}


}   // namespace hrd11

