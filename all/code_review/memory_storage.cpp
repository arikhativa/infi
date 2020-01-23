#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

#include "driver_data.hpp"
#include "storage.hpp"
#include "memory_storage.hpp"

namespace hrd11
{

MemoryStorage::MemoryStorage(size_t size) : m_disk(size)
{}

MemoryStorage::~MemoryStorage()
{}

std::unique_ptr<DriverData> MemoryStorage::Write(std::unique_ptr<DriverData> data)
{
    std::cout << "Request for write of size " << data->m_len << std::endl;

    std::copy(data->m_buff.begin(), data->m_buff.begin() + data->m_len,(m_disk.begin()+ data->m_offset));
    
    return std::move(data);
}
std::unique_ptr<DriverData> MemoryStorage::Read(std::unique_ptr<DriverData> data)
{
    std::cout << "Request for read of size " << data->m_len << std::endl;

    std::copy(m_disk.begin() + data->m_offset , m_disk.begin() +data->m_len + data ->m_offset, data->m_buff.begin() ); 

    return std::move(data); 
}

} // namespace hrd11


