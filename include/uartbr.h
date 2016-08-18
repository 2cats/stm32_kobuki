#ifndef _USARTBUFRECEIVER_H_
#define _USARTBUFRECEIVER_H_
#include "stm32f10x.h"
#include "usart.h"
#define USART_BUFSIZE 512

void UartbrInit(u32 bound);
unsigned int UartbrGetAvaliableNum(void);
u8 UartbrGetByte(void);
void UartbrGetBytes(u8 *dat,unsigned int num);
#endif
