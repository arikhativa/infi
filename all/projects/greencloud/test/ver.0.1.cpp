
#include <unistd.h>			// read()
#include <signal.h>			// sigaction

#include <memory>			// std::unique_ptr
#include <iostream>			// std::cerr


#include "nbd_driver_proxy.hpp"
#include "memory_storage.hpp"
#include "epoll.hpp"
#include "monitor.hpp"


using namespace hrd11;

static const unsigned int CONVTER_TO_MB = 1024 * 1024;
static const unsigned int NUM_OF_EVENTS = 1;
static const unsigned int BUFF_SIZE = 100;
static const unsigned int END_PROG = 1; // random number
static int s_run = 1;

enum ExitStatus
{
	SUCCESS = 0,
	ARGUMENTS = 1,
	EXCEPTION = 2
};

static void ExitLoop(int sig)
{
	(void)sig;
}

static void MainThreadSetSignals()
{
	struct sigaction act;
	int stt = 0;

	act.sa_handler = ExitLoop;
	act.sa_flags = 0;

	stt = sigemptyset(&act.sa_mask);
	if (stt)
	{
		throw std::runtime_error("fail to set signals");
	}
	stt = sigaddset(&act.sa_mask, SIGINT);
	if (stt)
	{
		throw std::runtime_error("fail to set signals");
	}
	stt = sigaddset(&act.sa_mask, SIGTERM);
	if (stt)
	{
		throw std::runtime_error("fail to set signals");
	}

	stt = sigaction(SIGINT, &act, NULL);
	if (stt)
	{
		throw std::runtime_error("fail to set signals");
	}
	stt = sigaction(SIGTERM, &act, NULL);
	if (stt)
	{
		throw std::runtime_error("fail to set signals");
	}
}

static void PrintUsege()
{
	std::cerr << "-- usage green_cloud.0.1.out:\t";
	std::cerr << "size_of_storage, path_to_fd" << std::endl;
	std::cerr << "-- usage green_cloud.0.1.out:\t";
	std::cerr << "size_of_block num_of_blocks path_to_fd" << std::endl;
}

static int HundelInputFD()
{
	char input[BUFF_SIZE];
	int stt = read(STDIN_FILENO, &input,  BUFF_SIZE);

	if (0 > stt)
	{
		throw std::runtime_error("read() fail");
	}

	return ('q' == input[0]);
}


int main(int ac, char* av[])
{
	std::unique_ptr<DriverProxy> driver;
	std::unique_ptr<Storage> storage;
	std::unique_ptr<DriverData> data;
	std::unique_ptr<Monitor> monitor;
	int ready_fd = 0;

	try
	{
		MainThreadSetSignals();
		switch (ac)
		{
			case 3:
			{
				size_t size = std::stoull(av[1]);

				size *= CONVTER_TO_MB;

				driver.reset(new NBDDriverProxy(size, av[2]));
				storage.reset(new MemoryStorage(size));
				break ;
			}
			case 4:
			{
				size_t blk_size = std::stoull(av[1]);
				size_t num_of_blocks = std::stoull(av[2]);

				driver.reset(new NBDDriverProxy(blk_size, num_of_blocks, av[3]));
				storage.reset(new MemoryStorage(blk_size * num_of_blocks));
				break ;
			}
			default:
				PrintUsege();
				exit(ARGUMENTS);
		}

		monitor.reset(new Epoll(NUM_OF_EVENTS));
		monitor->Add(STDIN_FILENO);
		monitor->Add(driver->GetReqFd());

		while (s_run)
		{
			ready_fd = monitor->Wait();

			if (STDIN_FILENO == ready_fd)
			{
				if (END_PROG == HundelInputFD())
				{
					driver->Disconnect();
					goto exit_safe;
				}
			}
			else
			{
				data = driver->ReceiveRequest();
				switch (data->m_type)
				{
					case READ:
					{
						data = storage->Read(std::move(data));
						break ;
					}
					case WRITE:
					{
						data = storage->Write(std::move(data));
						break ;
					}
					case DISCONNECT:
					{
						driver->Disconnect();
						goto exit_safe;
					}
					default:
					// TODO how hundel an unxepected request?
					break ;
				}

				driver->SendReply(std::move(data));
			}
		}
	}
	catch (std::runtime_error& e)
	{
		if (EINTR == errno)	// when epoll wakes from signal
		{
			driver->Disconnect();
			goto exit_safe;
		}
		std::cout << e.what();
		exit(EXCEPTION);
	}

	exit_safe:

	return SUCCESS;
}
