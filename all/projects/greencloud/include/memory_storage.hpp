
#ifndef __HRD11_MEMORY_STORAGE_HPP__
#define __HRD11_MEMORY_STORAGE_HPP__

#include <memory>		// std::unique_ptr
#include <vector>		// std::vector

#include "storage.hpp"
#include "driver_data.hpp"

namespace hrd11
{

class MemoryStorage : public Storage
{
public:
	// uncopiable through inheritence
	MemoryStorage(size_t size);
	virtual ~MemoryStorage() = default;

	virtual std::unique_ptr<DriverData> Write(std::unique_ptr<DriverData> data) override;
	virtual std::unique_ptr<DriverData> Read(std::unique_ptr<DriverData> data) override;

private:
	std::vector<char> m_disk;
};

}	// end namespace hrd11

#endif // __HRD11_MEMORY_STORAGE_HPP__
