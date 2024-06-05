#include "uart.h"

void uart_init()
{
	SIM->SCGC4 |= (1 << 10);
	SIM->SCGC5 |= (1 << 9);
	// set clock
	SIM->SOPT2 |= (1 << 26) | (1 << 16);

	// 8 9 10 -> 0 1 0 -> al2
	PORTA->PCR[1] |= (1 << 9);
	PORTA->PCR[2] |= (1 << 9);

	// disable UART
	UART0->C2 &= ~(1 << 3 | 1 << 2);

	uint16_t sbr = SystemCoreClock / (16 * 115200);
	UART0->BDH = sbr & 0x1F00; // 5 bit cao
	UART0->BDL = sbr & 0xFF;   // 8 bit thap

	UART0->C2 |= (1 << 3 | 1 << 2);
}
void uart_tx(char c)
{
	// check transmit c� bi full hay khong
	while ((UART0->S1 & (1 << 7)) == 0)
	{
	}
	UART0->D = c;
}
void uart_send_msg(char str[])
{
	uint32_t i = 0;
	while (str[i] != '\0')
	{
		uart_tx(str[i]);
		i++;
	}
}