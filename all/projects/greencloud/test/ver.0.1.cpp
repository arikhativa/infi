
#include <memory>			// std::unique_ptr
#include <iostream>			// std::cerr

#include "nbd_driver_proxy.hpp"
#include "memory_storage.hpp"

using namespace hrd11;

enum ExitStatus
{
	SUCCESS = 0,
	ARGUMENTS = 1
};

static int SetSigaction(int sig, const struct sigaction* act)
{
	int ret = 0;
	(void)sig;
	(void)act;
	// struct sigaction oact;
	//
	// ret = sigaction(sig, act, &oact);
	//
	// if (0 == ret && oact.sa_handler != SIG_DFL)
	// {
	// 	perror("NBDDriverProxy Ctor - SetSigaction() fail\n");
	// }

	return ret;
}

static void MainThreadSetSignals()
{
	// struct sigaction act;
	// int stt = 0;
	//
	// act.sa_handler = Disconnect_NBD;
	// act.sa_flags = SA_RESTART;
	//
	// stt = sigemptyset(&act.sa_mask);
	// stt += sigaddset(&act.sa_mask, SIGINT);
	// stt *= 100;
	// stt += sigaddset(&act.sa_mask, SIGTERM);
	// if (stt)
	// {
	// 	perror("NBDDriverProxy Ctor - MainThreadSetSignals(1)  fail\n");
	// }
	//
	// stt = SetSigaction(SIGINT, &act);
	// stt += SetSigaction(SIGTERM, &act);
	// if (stt)
	// {
	// 	perror("NBDDriverProxy Ctor - MainThreadSetSignals(2) fail\n");
	// }
}

static void PrintUsege()
{
	std::cerr << "-- usage green_cloud.0.1.out:";
	std::cerr << "size_of_storage path_to_fd" << std::endl;
	std::cerr << "-- usage green_cloud.0.1.out:";
	std::cerr << "size_of_block num_of_blocks path_to_fd" << std::endl;
}

int main(int ac, char* av[])
{
	std::unique_ptr<DriverProxy> driver;
	std::unique_ptr<Storage> storage;
	std::unique_ptr<DriverData> data;

	switch (ac)
	{
		case 3:
		{
			size_t size = std::stoull(av[1]);

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

	while (1)
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
				printf("here\n");
				data = storage->Write(std::move(data));
				break ;
			}
			case DISCONNECT:
			{
				driver->Disconnect();
				break ;
			}
			case TRIM:
				break ;
			case FLUSH:
				break ;
			default:
			{
				printf("-- main() bad Receive\n");
				exit(1);
			}
		}
		if (DISCONNECT == data->m_type)
		{
			break ;
		}

		driver->SendReply(std::move(data));
	}

	return SUCCESS;
}
