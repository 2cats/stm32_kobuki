/*******************************************************************************
 * Copyright (C) 2016 Ence Wang
 *
 * MIT License
 *******************************************************************************/
#ifndef _USARTBUFRECEIVER_H_
#define _USARTBUFRECEIVER_H_
#include "stm32f10x.h"
#include "usart.h"
#define USART_BUFSIZE 512
void Uart1brInit(u32 bound);
unsigned int Uart1brGetAvaliableNum(void);
u8 Uart1brGetByte(void);
void Uart1brGetBytes(u8 *dat,unsigned int num);
#define USART1SendBytes(p_ch,count) USARTSendBytes(USART1,p_ch,count)
#define USART1SendByte(ch) USARTSendByte(USART1,ch);
#endif
