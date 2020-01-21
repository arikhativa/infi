
#ifndef __HRD11_STORAGE_HPP__
#define __HRD11_STORAGE_HPP__

namespace hrd11
{

class Storage
{
public:
	// uncopiable
	Storage() = default;

	virtual void Write(std::unique_ptr<DriverData> data) = 0;
	virtual void Read(std::unique_ptr<DriverData> data) = 0;
};

}	// namespace hrd11

#endif // __HRD11_STORAGE_HPP__
