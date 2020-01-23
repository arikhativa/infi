#include <iostream>
#include <signal.h>
#include <sys/ioctl.h>
#include <linux/nbd.h>

#include "driver_proxy.hpp"
#include "nbd_driver_proxy.hpp"
#include "storage.hpp"
#include "memory_storage.hpp"
#include "select.hpp"

using namespace hrd11;

DriverProxy* proxy = 0;
Storage* disk = 0;
Monitor* selector = 0;


/************************** SIGNALS HANDLER FUNCTIONS ****************************/


static void disconnect_nbd(int signal)
{
    std::cout << "\nbye bye\n" << std::endl;

    delete proxy;
    delete disk;
    delete selector;
    exit(signal);
}


static int set_sigaction(int sig, const struct sigaction * act)
{
    struct sigaction oact;
    int r = sigaction(sig, act, &oact);
    if (r == 0 && oact.sa_handler != SIG_DFL)
    {
        return -1;
    }
    return 0;
}

static void InitSignals()
{
    struct sigaction act; 
    act.sa_handler = disconnect_nbd;
    act.sa_flags = SA_RESTART; 
    if(sigemptyset(&act.sa_mask)||
            sigaddset(&act.sa_mask, SIGINT)||
                    sigaddset(&act.sa_mask, SIGTERM))
    {
        throw NBDError("main thread sigmask failed.");
    }
    if((-1 == set_sigaction(SIGINT, &act)) || (-1 == set_sigaction(SIGTERM, &act)))
    {
        throw NBDError("main thread sigmask failed.");
    }
}

/************************** REACTOR FUNCTIONS ****************************/


int Quit()
{
    char cmd[10] = {0};
    read(STDIN_FILENO,cmd, 10);
    if(!strcmp(cmd, "quit\n"))
    {
        disconnect_nbd(0);
    }
    return 0;
}

int Request()
{
    auto data_ptr = proxy->ReceiveRequest();
    switch(data_ptr->m_type)
    {
        case READ: 
            std::cout << "Got NBD_CMD_READ" << std::endl;
            data_ptr = disk->Read(std::move(data_ptr));
            proxy->SendReply(std::move(data_ptr));
            break;
        
        case WRITE:
            std::cout << "Got NBD_CMD_WRITE" << std::endl;
            data_ptr = disk->Write(std::move(data_ptr));
            proxy->SendReply(std::move(data_ptr));
            break;
        
        case FLUSH:
            std::cout << "Got NBD_CMD_FLUSH" << std::endl;
            proxy->SendReply(std::move(data_ptr));
            break;
        
        case TRIM:
            std::cout << "Got NBD_CMD_TRIM" << std::endl;
            proxy->SendReply(std::move(data_ptr));
            break;

        case DISC:
            std::cout << "Got NBD_CMD_DISC" << std::endl;
            disconnect_nbd(0);
    };

    return 0;
}

/**************************** APP STATIC FUNCTIONS ****************************/

static size_t GetSize(char* str)
{
    char end = *(str+strlen(str)-1);
    *(str+strlen(str)-1) = '\0';
    size_t ret = atoi(str);
    switch (end) 
    {
        case 'K':
            ret *= 1024;
            break;
        case 'M':
            ret *= 1024 * 1024;
            break;
        case 'G':
            ret *= 1024 * 1024 * 1024;
            break;
    };
    return ret;
}

/****************************** MAIN ****************************************/


int main(int argc, char** argv)
{
    (void)argc;
    size_t size = GetSize(argv[1]);

    InitSignals();

    try
    {
        proxy = new NBDDriverProxy(size, argv[2]);
        disk = new MemoryStorage(size);
        selector = new Epoll();
        selector->AddFd(STDIN_FILENO,Quit);
        selector->AddFd(proxy->GetReqFd(),Request);

        selector->RunReactor();
    }
    catch(NBDError&  e)
    {
        std::cerr << e.what() << '\n';
        delete proxy;
        delete disk;
        delete selector;
    }
 
    return 0;
}