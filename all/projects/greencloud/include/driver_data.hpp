
#ifndef __HRD11_DRIVER_PROXY_HPP__
#define __HRD11_DRIVER_PROXY_HPP__

static const int HANDLE_SIZE = 8;

struct DriverData
{
	size_t m_offset;   
	char m_handler[HANDLE_SIZE];
	unsigned int m_len;	
	int m_type; // make enum insted of int
	int m_status; // describe expected status
	std::vector<char> m_buff;
};

#endif // __HRD11_DRIVER_PROXY_HPP__
