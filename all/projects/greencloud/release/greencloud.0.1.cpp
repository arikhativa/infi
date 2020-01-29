
// C include ------------------------------------------------------------------
#include <unistd.h>			// read()
#include <signal.h>			// sigaction
#include <errno.h>      	// errno

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
static void RunGreenCloude(std::shared_ptr<DriverProxy> driver,
							std::shared_ptr<Storage> storage);

enum ExitStatus
{
	SUCCESS = 0,
	ARGUMENTS = 1,
	EXCEPTION = 2
};

enum RunLoop
{
	RUN = 0,
	STOP
};


// Global ---------------------------------------------------------------------

static const unsigned int CONVTER_TO_MB = 1024 * 1024;
static const unsigned int MAX_EVENTS_TO_WAKE = 1;
static const unsigned int BUFF_SIZE = 100;
static const unsigned int END_PROG = 1;
static int s_run_loop = RUN;


// Main -----------------------------------------------------------------------

int main(int ac, char* av[])
{
	std::shared_ptr<DriverProxy> driver;
	std::shared_ptr<Storage> storage;

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

		RunGreenCloude(driver, storage);

	}
	catch (EpollError& e)
	{
		if (EINTR == e.m_errno)	// Epoll throws exception when a signal is recived
		{
			driver->Disconnect();
			goto exit_safe;
		}
	}
	catch (std::runtime_error& e)
	{
		std::cerr << e.what();
		exit(EXCEPTION);
	}

	exit_safe:

	return SUCCESS;
}

// Static funcs ---------------------------------------------------------------

static void ThreadFunc(std::shared_ptr<DriverProxy> driver,
							std::shared_ptr<Storage> storage,
							std::unique_ptr<DriverData> data)
{
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
			s_run_loop = STOP;
			goto exit_safe;
		}
		default:
		break ;
	}

	driver->SendReply(std::move(data));

	exit_safe:

	return ;
}

static void RunGreenCloude(std::shared_ptr<DriverProxy> driver,
							std::shared_ptr<Storage> storage)
{
	std::unique_ptr<Monitor> monitor(new Epoll(MAX_EVENTS_TO_WAKE));
	std::unique_ptr<DriverData> data;

	monitor->Add(STDIN_FILENO, Epoll::READ_FD);
	monitor->Add(driver->GetReqFd(), Epoll::READ_FD);

	while (RUN == s_run_loop)
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

			ThreadFunc(driver, storage, std::move(data));

		}
	}

	exit_safe:

	return ;

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
	int stt = read(STDIN_FILENO, &input, BUFF_SIZE);

	if (0 > stt)
	{
		Logger* log = Handleton<Logger>::GetInstance(LOG_PATH, LOG_LVL);
		log->Write(LOG_ERROR, "HundelInputFD() fail to read from stdin",
								__FILE__, __LINE__);

		throw std::runtime_error("HundelInputFD() fail to read from stdin");
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
		Logger* log = Handleton<Logger>::GetInstance(LOG_PATH, LOG_LVL);
		log->Write(LOG_ERROR, "SigAdd() fail to set signals",
					__FILE__, __LINE__);

		throw std::runtime_error("SigAdd() - fail to set signals");
	}
}

static void Sigaction(struct sigaction& act, int sig)
{
	int stt = 0;

	stt = sigaction(sig, &act, NULL);
	if (stt)
	{
		Logger* log = Handleton<Logger>::GetInstance(LOG_PATH, LOG_LVL);
		log->Write(LOG_ERROR, "Sigaction() fail to set signals",
					__FILE__, __LINE__);

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
		Logger* log = Handleton<Logger>::GetInstance(LOG_PATH, LOG_LVL);
		log->Write(LOG_ERROR, "SigEmpty() fail to set signals",
					__FILE__, __LINE__);

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
		Logger* log = Handleton<Logger>::GetInstance(LOG_PATH, LOG_LVL);
		log->Write(LOG_ERROR, "SetSigMask() fail to set signals",
					__FILE__, __LINE__);

		throw std::runtime_error("SetSigMask() fail to set signals");
	}
}

static void MainThreadSetSignals()
{
	Logger* log = Handleton<Logger>::GetInstance(LOG_PATH, LOG_LVL);
	log->Write(LOG_DEBUG, "setting main thread signls", __FILE__, __LINE__);

	struct sigaction act;

	SetSigMask();

	SigEmpty(act);
	SigAdd(act, SIGINT);
	SigAdd(act, SIGTERM);
	Sigaction(act, SIGINT);
	Sigaction(act, SIGTERM);
	log->Write(LOG_INFO, "main thread signls are set", __FILE__, __LINE__);
}
