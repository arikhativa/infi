
#include <dlfcn.h>	/* dlopen() && dlclose() && dlsym */

typedef void (*fp_t)(void);

void PrintYalla(void);

int main()
{
	void *handle = 0;
	fp_t func = 0;
	
	handle = dlopen("/usr/lib/libhey.so", RTLD_LAZY);
	func = dlsym(handle, "PrintHey");

	if (!func)
	{
		return -1;
	}

	func();
	PrintYalla();

	dlclose(handle);
	func = 0;

	return 0;
}
