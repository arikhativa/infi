
#include <stddef.h>

#define SIZEOF_VAR(VAR) ((size_t)(&(VAR) + 1) - (size_t)&(VAR))
