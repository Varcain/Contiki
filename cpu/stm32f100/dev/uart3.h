#ifndef __UART3_H__
#define __UART3_H__

#include "contiki.h"

void uart3_set_input(int (*input)(unsigned char c));
void uart3_writeb(unsigned char c);
void uart3_init(unsigned long ubr);
uint8_t uart3_active(void);

#endif /* __UART3_H__ */
