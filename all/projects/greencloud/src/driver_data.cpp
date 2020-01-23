
// delll
#include <stdio.h>


#include <cstring>      // memcpy()
#include <arpa/inet.h>	// ntohl(), htonl()
#include <unistd.h>	    // read()


#include "driver_data.hpp"

namespace hrd11
{

DriverData::DriverData()
{}

DriverData::DriverData(unsigned int len) : m_len(len), m_buff(len)
{}

void DriverData::SetInfo(size_t offset, char handler[HANDLE_SIZE], DataType type)
{
    m_offset = offset;

    memcpy(m_handler, handler, HANDLE_SIZE);

	m_type = type;
	m_status = htonl(0);
}


}   // end namespace hrd11
