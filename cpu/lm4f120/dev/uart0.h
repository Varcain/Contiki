#ifndef __UART0_H__
#define __UART0_H__

#include "contiki.h"

void uart0_set_input(int (*input)(unsigned char c));
void uart0_writeb(unsigned char c);
void uart0_init(unsigned long ubr);
uint8_t uart0_active(void);

#endif /* __UART0_H__ */
