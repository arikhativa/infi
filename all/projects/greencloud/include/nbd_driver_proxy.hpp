
#ifndef __HRD11_NBD_DRIVER_PROXY_HPP__
#define __HRD11_NBD_DRIVER_PROXY_HPP__

#include <thread>			//std::thread

#include "driver_proxy.hpp"
#include "driver_data.hpp"
#include "semaphore.hpp"

namespace hrd11
{

class NBDDriverProxy : public DriverProxy
{
public:
	// no cctor & cpy=. uncopiable through inheritence.

	// 'storage_size' is by MB
	NBDDriverProxy(size_t storage_size, const std::string& device_name);
	// 'block_size' is by MB
	NBDDriverProxy(size_t block_size, size_t num_blocks, const std::string& device_name);
	~NBDDriverProxy() override;

	std::unique_ptr<DriverData> ReceiveRequest() override;
	void SendReply(std::unique_ptr<DriverData> data) override;
	void Disconnect() override;
	int GetReqFd() override;

private:
	std::string m_device_name;
	int m_req_fd; // request
	int m_nbd_fd;
	int m_file_fd;
	std::thread m_nbd_thread;
	Semaphore m_sem;

};


}   // namespace hrd11

#endif // __HRD11_NBD_DRIVER_PROXY_HPP__
