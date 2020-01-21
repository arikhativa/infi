
#ifndef __HRD11_MEMORY_STORAGE_HPP__
#define __HRD11_MEMORY_STORAGE_HPP__

#include "storage.hpp"

namespace hrd11
{

class MemoryStorage : public Storage
{
public:
	// uncopiable through inheritence
	MemoryStorage(size_t size);
	~MemoryStorage() // maye not needed?

	virtual void Write(std::unique_ptr<DriverData> data);
	virtual void Read(std::unique_ptr<DriverData> data);

private:
	std::array<char> m_disk; // fixed size array in size recived by ctor
};

}	// namespace hrd11

#endif // __HRD11_MEMORY_STORAGE_HPP__
