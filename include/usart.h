#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"

#define EN_USART1_RX 			0		//使能（1）/禁止（0）串口1接收
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void usart_send_bytes(const unsigned char *ch,int count);
void usart_send_byte(char c);
#endif




