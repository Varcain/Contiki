#ifndef __UART1_H__
#define __UART1_H__

#include "contiki.h"

void uart1_set_input(int (*input)(unsigned char c));
void uart1_writeb(unsigned char c);
void uart1_init(unsigned long ubr);
uint8_t uart1_active(void);

#endif /* __UART1_H__ */
