
/*
    ver 1.2     Print is working with '%'.
    ver 1.1     Nice is nice.
*/

#ifndef __WRITE_PRINT_H__
#define __WRITE_PRINT_H__

#include <stddef.h>     /* size_t */
#include <sys/types.h>  /* ssize_t */

#ifdef _DONT_DEFINE_

    Possibale Actions:
    Spicial Char    |   Action        | input

    %s              |   Print string  |  char *
    %d              |   Print number  |  int
    %ld             |   Print number  |  ssize_t
    %u              |   Print number  |  unsigned int
    %lu             |   Print number  |  size_t
    %x              |   Print hexa    |  size_t
    %b              |   Print binary  |  unsigned int
    %cb             |   Print binary  |  unsigned char
    %lb             |   Print binary  |  size_t
    %m              |   Print bitmap  |  size_t
    %a              |   Print array   |  int * + size_t (ptr & arr_size)
    %la             |   Print array   |  size_t * + size_t (ptr & arr_size)


#endif /* _DONT_DEFINE_ */

void Print(char *info, ...);

#endif /*__WRITE_PRINT_H__*/
