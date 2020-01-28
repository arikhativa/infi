
// C include ------------------------------------------------------------------
#include <unistd.h>			// read()
#include <signal.h>			// sigaction

// C++ include ----------------------------------------------------------------
#include <memory>			// std::unique_ptr
#include <iostream>			// std::cerr

// local include --------------------------------------------------------------
#include "nbd_driver_proxy.hpp"
#include "memory_storage.hpp"
#include "epoll.hpp"
#include "monitor.hpp"
#include "handleton.hpp"
#include "logger.hpp"
#include "globals.hpp"

using namespace hrd11;

// Declarations ---------------------------------------------------------------

static void PrintUsege();
static int HundelInputFD();

static void ExitLoop(int sig);
static void SigAdd(struct sigaction& act, int sig);
static void Sigaction(struct sigaction& act, int sig);
static void SigEmpty(struct sigaction& act);
static void SetSigMask();
static void MainThreadSetSignals();

enum ExitStatus
{
	SUCCESS = 0,
	ARGUMENTS = 1,
	EXCEPTION = 2
};


// Global ---------------------------------------------------------------------

static const unsigned int CONVTER_TO_MB = 1024 * 1024;
static const unsigned int MAX_EVENTS_TO_WAKE = 1;
static const unsigned int BUFF_SIZE = 100;
static const unsigned int END_PROG = 1;


// Main -----------------------------------------------------------------------

int main(int ac, char* av[])
{
	std::unique_ptr<DriverProxy> driver;
	std::unique_ptr<Storage> storage;
	std::unique_ptr<DriverData> data;
	std::unique_ptr<Monitor> monitor;

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

		monitor.reset(new Epoll(MAX_EVENTS_TO_WAKE));
		monitor->Add(STDIN_FILENO, Epoll::READ_FD);
		monitor->Add(driver->GetReqFd(), Epoll::READ_FD);

		while (1)
		{
			monitor->Wait();

			if (STDIN_FILENO == (*monitor)[0])
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
					// TODO create a logger for an unexpected Request
					break ;
				}

				driver->SendReply(std::move(data));
			}
		}
	}
	catch (std::runtime_error& e)
	{
		if (EINTR == errno)	// Epoll throws exception when a signal is recived
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

// Static funcs ---------------------------------------------------------------

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


// Signals --------------------------------------------------------------------

static void ExitLoop(int sig)
{
	(void)sig;
}

static void SigAdd(struct sigaction& act, int sig)
{
	int stt = 0;

	stt = sigaddset(&act.sa_mask, sig);
	if (stt)
	{
		throw std::runtime_error("SigAdd() - fail to set signals");
	}
}

static void Sigaction(struct sigaction& act, int sig)
{
	int stt = 0;

	stt = sigaction(sig, &act, NULL);
	if (stt)
	{
		throw std::runtime_error("Sigaction() fail to set signals");
	}
}

static void SigEmpty(struct sigaction& act)
{
	int stt = 0;

	act.sa_handler = ExitLoop;
	act.sa_flags = 0;

	stt = sigemptyset(&act.sa_mask);
	if (stt)
	{
		throw std::runtime_error("SigEmpty() fail to set signals");
	}
}

static void SetSigMask()
{
	int stt = 0;

	sigset_t mask;

	stt = sigfillset(&mask);
	stt += sigdelset(&mask, SIGINT);
	stt += sigdelset(&mask, SIGTERM);
	stt += sigprocmask(SIG_SETMASK, &mask, 0);

	if (stt)
	{
		throw std::runtime_error("fail to set signals");
	}
}

static void MainThreadSetSignals()
{
	Logger* log = Handleton<Logger>::GetInstance(LOG_PATH, LOG_LVL);
	log->Write(LOG_INFO, "main thread signls are set", __FILE__, __LINE__);

	struct sigaction act;

	SetSigMask();

	SigEmpty(act);
	SigAdd(act, SIGINT);
	SigAdd(act, SIGTERM);
	Sigaction(act, SIGINT);
	Sigaction(act, SIGTERM);
}
