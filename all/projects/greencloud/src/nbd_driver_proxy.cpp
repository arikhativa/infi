
// del this
#include <stdio.h>


#include <unistd.h>			// close()
#include <sys/types.h>		// socketpair(), open()
#include <sys/socket.h>		// socketpair()
#include <signal.h>			// sigfilset(), sigprocmask()
#include <sys/ioctl.h>		// ioctl()
#include <linux/nbd.h>		// NBD macros
#include <sys/stat.h>		// open()
#include <fcntl.h>			// open()
#include <arpa/inet.h>		// ntohl(), htonl()
#include <string>			// std::string
#include <memory>			// std::unique_ptr
#include <cstring>	        // memcpy()
#include <iostream>			// std::cout

#include "nbd_driver_proxy.hpp"
#include "driver_data.hpp"
#include "ioctl_wrapper.hpp"

namespace hrd11
{

// Declarations ---------------------------------------------------------------

static void WriteAll(int fd, char* buff, unsigned int count);
static void ReadAll(int fd, char* buff, unsigned int count);
static size_t Ntohll(size_t a);

static void ThreadFuncSetNBD(int file_fd, int socket_fd);
static void SetFlags(int file_fd);
static void SubThreadSetSignals();

static void InitSockets(int* req, int* nbd);
static void InitDevice(int* file_fd, size_t blk_size,
						size_t num_blks, const std::string& device_name);

enum SocketName
{
	REQUEST = 0,
	NBD = 1
};

enum OffOn
{
	ON = 0,
	OFF = 1
};

// Global ---------------------------------------------------------------------

static const unsigned int DEFAULT_NUM_BLOCKS = 1024;

// Special Members ------------------------------------------------------------

NBDDriverProxy::NBDDriverProxy(
	size_t storage_size, const std::string& device_name) :
	NBDDriverProxy::NBDDriverProxy(
		DEFAULT_NUM_BLOCKS , storage_size / DEFAULT_NUM_BLOCKS, device_name)
{}

NBDDriverProxy::NBDDriverProxy(
	size_t block_size, size_t num_blocks, const std::string& device_name) :
	m_device_name(device_name),
	m_connected(ON)
{
	InitSockets(&m_req_fd, &m_nbd_fd);
	InitDevice(&m_file_fd, block_size , num_blocks, device_name);

	m_nbd_thread = std::thread(ThreadFuncSetNBD, m_file_fd, m_nbd_fd);
}

NBDDriverProxy::~NBDDriverProxy()
{
	if (m_connected)
	{
		Disconnect();
	}
}

std::unique_ptr<DriverData> NBDDriverProxy::ReceiveRequest()
{
	struct nbd_request request;
	ssize_t bytes_read = 0;
	std::unique_ptr<DriverData> ret(new DriverData(0));

	memset(&request, 0, sizeof(request));

	bytes_read = read(m_req_fd, &request, sizeof(request));
	if (0 > bytes_read)
	{
		throw std::runtime_error("ReceiveRequest()\nfail to read() requet from socket");
	}

	if (request.magic != htonl(NBD_REQUEST_MAGIC))
	{
		throw std::runtime_error("ReceiveRequest()\ninvalivd magic number");
	}

	unsigned int len = ntohl(request.len);

	memcpy(ret->m_handler, request.handle, sizeof(ret->m_handler));
	ret->m_status = htonl(0);

	switch(ntohl(request.type))
	{
		case NBD_CMD_READ:

			ret->m_offset = Ntohll(request.from);
			ret->m_len = len;
			ret->m_type = READ;
			break ;

		case NBD_CMD_WRITE:

			ret->m_offset = Ntohll(request.from);
			ret->m_len = len;
			ret->m_type = WRITE;
			ret->m_buff.reserve(len);
			ReadAll(m_req_fd, ret->m_buff.data(), len);
			break ;

		case NBD_CMD_DISC:

			ret->m_type = DISCONNECT;
			break ;

		case NBD_CMD_FLUSH:

			ret->m_type = FLUSH;
			break ;

		case NBD_CMD_TRIM:

			ret->m_type = TRIM;
			break ;

		default:
			ret->m_type = BAD_REQUEST;
	}

	return std::move(ret);
}

static void InitReaply(struct nbd_reply* reply)
{
	memset(reply, 0, sizeof(struct nbd_reply));
	reply->magic = htonl(NBD_REPLY_MAGIC);
}

void NBDDriverProxy::SendReply(std::unique_ptr<DriverData> data)
{
	struct nbd_reply reply;

	InitReaply(&reply);

	reply.error = data->m_status;
	memcpy(reply.handle, data->m_handler, sizeof(reply.handle));

	WriteAll(m_req_fd, (char*)&reply, sizeof(struct nbd_reply));

	if (READ == data->m_type)
	{
		WriteAll(m_req_fd, data->m_buff.data(), data->m_len);
	}
}

void NBDDriverProxy::Disconnect()
{
	Ioctl(m_file_fd, NBD_DISCONNECT, IoctlError::NO_FLAGS);

	// the order is crucial. closing the fds will help the thread to exit.
	close(m_req_fd);
	close(m_nbd_fd);

	m_nbd_thread.join();

	close(m_file_fd);

	m_connected = 0;
}

int NBDDriverProxy::GetReqFd()
{
	return m_req_fd;
}

// Thread ---------------------------------------------------------------------

static void ThreadFuncSetNBD(int file_fd, int socket_fd)
{
	SubThreadSetSignals();
	try
	{
		Ioctl(file_fd, NBD_SET_SOCK, socket_fd);

		SetFlags(file_fd);

		Ioctl(file_fd, NBD_DO_IT, IoctlError::NO_FLAGS);
		Ioctl(file_fd, NBD_CLEAR_QUE, IoctlError::NO_FLAGS);
		Ioctl(file_fd, NBD_CLEAR_SOCK, IoctlError::NO_FLAGS);
	}
	catch (std::runtime_error& e)
	{
		std::cout << e.what();
		// TODO Before Exit let the main thread know.
		exit(-1);
	}
}

// Static Functions -----------------------------------------------------------

#ifdef WORDS_BIGENDIAN
static size_t Ntohll(size_t a)
{
  return a;
}
#else
static size_t Ntohll(size_t a)
{
	unsigned int lo = a & 0xffffffff;
	unsigned int hi = a >> 32U;

	lo = ntohl(lo);
	hi = ntohl(hi);

	return ((size_t) lo) << 32U | hi;
}
#endif
#define htonll ntohll

static void ReadAll(int fd, char* buff, unsigned int count)
{
    int bytes_read;

    while (count > 0)
    {
        bytes_read = read(fd, buff, count);

        if (0 > bytes_read)
        {
			throw std::runtime_error("ReadAll()");
        }

        buff += bytes_read;
        count -= bytes_read;
    }
}

static void WriteAll(int fd, char* buff, unsigned int count)
{
    int bytes_written;

    while (count > 0)
    {
        bytes_written = write(fd, buff, count);

        if (0 > bytes_written)
        {
			throw std::runtime_error("WriteAll()");
        }

        buff += bytes_written;
        count -= bytes_written;
    }
}

static void SubThreadSetSignals()
{
	sigset_t sigset;
	int stt = 0;

	stt = sigfillset(&sigset);
	stt += pthread_sigmask(SIG_SETMASK, &sigset, NULL);

	if (stt)
	{
		 throw std::runtime_error("Thread failed to mask signals");
	}
}

static void InitSockets(int* req, int* nbd)
{
	int socket_pair[2];
	int stt = 0;

	stt = socketpair(AF_UNIX, SOCK_STREAM, 0, socket_pair);
	if (stt)
	{
		throw std::runtime_error("socketpair() fail");
	}

	*req = socket_pair[REQUEST];
	*nbd = socket_pair[NBD];
}

static void InitDevice(int* file_fd, size_t blk_size,
						size_t num_blks, const std::string& device_name)
{
	*file_fd = open(device_name.c_str(), O_RDWR);

	if (0 > *file_fd)
	{
		throw std::runtime_error("InitDevice()\nopen() fail");
	}

	Ioctl(*file_fd, NBD_SET_BLKSIZE, blk_size);
	Ioctl(*file_fd, NBD_SET_SIZE_BLOCKS, num_blks);
	Ioctl(*file_fd, NBD_CLEAR_SOCK, IoctlError::NO_FLAGS);
}

static void SetFlags(int file_fd)
{
	int flags = 0;

	#ifdef NBD_SET_FLAGS
		#ifdef NBD_FLAG_SEND_TRIM
		flags |= NBD_FLAG_SEND_TRIM;
		#endif
		#ifdef NBD_FLAG_SEND_FLUSH
		flags |= NBD_FLAG_SEND_FLUSH;
		#endif
		if (flags != 0)
		{
			Ioctl(file_fd, NBD_SET_FLAGS, flags);
		}
	#endif
}

}   // namespace hrd11
