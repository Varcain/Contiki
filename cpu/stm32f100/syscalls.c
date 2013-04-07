#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
// hardware includes
#include "uart3.h"

#undef errno
extern int errno;

/*
 read
 Read a character to a file. `libc' subroutines will use this system routine for input from all files, including stdin
 Returns -1 on error or blocks until the number of characters have been read.
 */
int _read(int file, char *ptr, int len) {
    int n;
    int num = 0;
    switch (file) {
    case STDIN_FILENO:
        for (n = 0; n < len; n++) {
            //while ((USART1->SR & USART_FLAG_RXNE) == (uint16_t)RESET) {}
            //char c = (char)(USART1->DR & (uint16_t)0x01FF);
        	char c=0;
            *ptr++ = c;
            num++;
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return num;
}

/*
 write
 Write a character to a file. `libc' subroutines will use this system routine for output to all files, including stdout
 Returns -1 on error or number of bytes sent
 */
int _write(int file, char *ptr, int len) {
    int n;
    switch (file) {
    case STDOUT_FILENO: /*stdout*/
        for (n = 0; n < len; n++)
        {
            uart3_writeb(*ptr++ & (uint16_t)0x01FF);
        }
        break;
    case STDERR_FILENO: /* stderr */
        for (n = 0; n < len; n++)
        {
            uart3_writeb(*ptr++ & (uint16_t)0x01FF);
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return len;
}

# define _LLIO_STDIN ((int) stdin)
# define _LLIO_STDOUT ((int) stdout)
# define _LLIO_STDERR ((int) stderr)
# define _LLIO_ERROR  (-1)


#undef putchar

int __attribute__(( weak )) putchar(int c)
{
  uart3_writeb(c);
  return c;
}

void __io_putchar(char c)
{
  putchar(c);
}
