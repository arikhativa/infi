#define _POSIX_C_SOURCE (200809L)

#include <assert.h>
#include <errno.h>
#include <err.h>
#include <fcntl.h>
#include <linux/nbd.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <fstream>
#include <cassert>
#include <functional>
#include <memory>
#include <argp.h>
#include <err.h>
#include <iostream>
#include <thread>
#include <algorithm>

#include "driver_data.hpp"
#include "driver_proxy.hpp"
#include "nbd_driver_proxy.hpp"


#ifdef WORDS_BIGENDIAN
u_int64_t ntohll(u_int64_t a)
{
  return a;
}
#else
    u_int64_t ntohll(u_int64_t a)
    {
        u_int32_t lo = a & 0xffffffff;
        u_int32_t hi = a >> 32U;
        lo = ntohl(lo);
        hi = ntohl(hi);
        return ((u_int64_t) lo) << 32U | hi;
    }
#endif
#define htonll ntohll


namespace hrd11
{
static int const BLOCK_SIZE = 1024;
/**************************************** Error Handle FUNCTIONS *************************************/


NBDError::NBDError(const std::string& error): std::runtime_error(error)
{}

std::string NBDError::what()
{
    return std::runtime_error::what();
}

/****************************************** STATIC FUNCTIONS *************************************/

template<typename... Args>
static void ControlDevice(int dev_fd, int op, Args... arg)
{
    if(ioctl(dev_fd, op, arg...) == -1)
    {
        throw NBDError("control failed : " + std::string(strerror(errno)));
    }
}


static void SetFlag(int& flags, int dev_fd)
{
    #ifdef NBD_SET_FLAGS
    flags = 0;
        #ifdef NBD_FLAG_SEND_TRIM
        flags |= NBD_FLAG_SEND_TRIM;
        #endif
        #ifdef NBD_FLAG_SEND_FLUSH
        flags |= NBD_FLAG_SEND_FLUSH;
        #endif 
        if(flags)
        {
            ControlDevice(dev_fd,NBD_SET_FLAGS,flags);
        }

    #endif  
}

static int ReadAll(int fd, char* buf, size_t sizeof_elem)
{
    int bytes_read = 0;

    while (sizeof_elem > 0)
    {
        bytes_read = read(fd, buf, sizeof_elem);
        assert(bytes_read > 0);
        buf += bytes_read;
        sizeof_elem -= bytes_read;
    }
    assert(sizeof_elem == 0);

  return 0;
}

static int WriteAll(int fd, char* buf, size_t sizeof_elem)
{
    int bytes_written = 0;

    while (sizeof_elem > 0)
    {
        bytes_written = write(fd, buf, sizeof_elem);
        assert(bytes_written > 0);
        buf += bytes_written;
        sizeof_elem -= bytes_written;
    }
    assert(sizeof_elem == 0);

  return 0;
}

static Type GetType(size_t type)
{
    switch(type)
    {
        case NBD_CMD_READ:
            return READ;
        case NBD_CMD_WRITE:
            return WRITE;
        case NBD_CMD_DISC:
            return DISC;
        case NBD_CMD_FLUSH:
            return FLUSH;
        case NBD_CMD_TRIM: 
            return TRIM;
    };
    return TRIM;
}

/**************************************** API FUNCTIONS *************************************/

NBDDriverProxy::NBDDriverProxy(size_t storage_size, const std::string& device_name) : NBDDriverProxy(BLOCK_SIZE ,storage_size / BLOCK_SIZE, device_name)
{}

NBDDriverProxy::NBDDriverProxy(size_t block_size, size_t num_blocks, const std::string& device_name) 
{
    BuildNBD(block_size, num_blocks, device_name);
}
NBDDriverProxy::~NBDDriverProxy()
{
    Disconnect();
}

std::unique_ptr<DriverData> NBDDriverProxy::ReceiveRequest()
{
    struct nbd_request request;
    memset(&request,0,sizeof(request));
    if(read(m_sock_fds[APP], &request, sizeof(request)) < 0)
    {
        throw NBDError("read request failed.");
    }
    
    size_t len = ntohl(request.len);

    std::unique_ptr<DriverData> ret_val(new DriverData(len));

    ret_val->m_offset = ntohll(request.from);
    memcpy(ret_val->m_handler, request.handle, sizeof(ret_val->m_handler));
    ret_val->m_status = htonl(0);

    if(request.magic != htonl(NBD_REQUEST_MAGIC))
    {
        throw NBDError("magic numbers doesnt fit.");
    }
  
    ret_val->m_type = GetType(ntohl(request.type));
    if(ret_val->m_type == WRITE)
    {
        ReadAll(m_sock_fds[APP], ret_val->m_buff.data(), len);
    }

    return std::move(ret_val);
}
void NBDDriverProxy::SendReply(std::unique_ptr<DriverData> data)
{
    struct nbd_reply reply;

    reply.magic = htonl(NBD_REPLY_MAGIC);
    reply.error  = data->m_status;
    memcpy(reply.handle, data->m_handler, sizeof(reply.handle));

    WriteAll(m_sock_fds[APP], reinterpret_cast<char*>(&reply), sizeof(reply));
    
    if(data->m_type == READ)
    {
        WriteAll(m_sock_fds[APP], data->m_buff.data(), data->m_len);
    }

}
void NBDDriverProxy::Disconnect()
{   
    ControlDevice(m_dev_fd, NBD_DISCONNECT);
    close(m_sock_fds[NBD]);
    close(m_sock_fds[APP]);
    close(m_dev_fd);

}
int NBDDriverProxy::GetReqFd()
{
    return m_sock_fds[APP];
}



/************************************** PRIVATE SERVICE FUNCTIONS *************************************/


void NBDDriverProxy::BlockNBDThread()
{
    try
    {
        sigset_t sigset;
        if(sigfillset(&sigset) || pthread_sigmask(SIG_SETMASK, &sigset, NULL))
        {
            throw NBDError("init thread sigmask failed.");
        }
        ControlDevice(m_dev_fd, NBD_SET_SOCK, m_sock_fds[NBD]);

        int flags = 0;
        SetFlag(flags,m_dev_fd);
        ControlDevice(m_dev_fd,NBD_DO_IT);
        ControlDevice(m_dev_fd,NBD_CLEAR_QUE);
        ControlDevice(m_dev_fd,NBD_CLEAR_SOCK); 
    }
    catch(NBDError& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void NBDDriverProxy::BuildNBD(int blksize, size_t size_blocks, const std::string& device_name)
{
    if(socketpair(AF_UNIX, SOCK_STREAM, 0, m_sock_fds))
    {
        throw NBDError("create socket failed.");
    }
    
    m_dev_fd = open(device_name.c_str(), O_RDWR);
    if(m_dev_fd == -1)
    {
        throw NBDError("open nbd failed.");
    }

    ControlDevice(m_dev_fd, NBD_SET_BLKSIZE, blksize);
    ControlDevice(m_dev_fd, NBD_SET_SIZE_BLOCKS, size_blocks);

    ControlDevice(m_dev_fd, NBD_CLEAR_SOCK);

    std::thread nbd_thread(&NBDDriverProxy::BlockNBDThread, this);
    nbd_thread.detach();
}

} // namespace hrd11


