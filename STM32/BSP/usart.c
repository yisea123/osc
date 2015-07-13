#include "stm32f10x.h"
#include "usart.h"
#include <stdio.h>

void USART1_Init(void)
{
	USART_InitTypeDef UART;
	GPIO_InitTypeDef GPIO;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA,ENABLE); /* Enable USART1 Clock */
	/* Config UART TX Pin */
	GPIO.GPIO_Pin   = GPIO_Pin_9;
	GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO);
	/* Config UART RX Pin */
	GPIO.GPIO_Pin   = GPIO_Pin_10;
	GPIO.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO);
	
	UART.USART_BaudRate			   = 115200;
	UART.USART_WordLength 		   = USART_WordLength_8b;
	UART.USART_StopBits   		   = USART_StopBits_1;
	UART.USART_Parity     		   = USART_Parity_No;
	UART.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	UART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &UART);
	USART_Cmd(USART1, ENABLE); /* Enable USART1 */
}

void UART1Send(unsigned char ch)
{
	USART_SendData(USART1, (uint8_t) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

#if 1
#pragma import(__use_no_semihosting)                              
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;         
/*_sys_exit(int x) 
{ 
	x = x; 
} */
int fputc(int ch, FILE *f)
{
  	UART1Send(ch);
  	return ch;
}
#endif

