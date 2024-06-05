#ifndef UART_H
#define UART_H

#include "MKL46Z4.h"

// default UART 0

void uart_init();
void uart_tx(char c);
void uart_send_msg(char str[]);

#endif