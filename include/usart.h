/*******************************************************************************
 * Copyright (C) 2016 Ence Wang
 *
 * MIT License
 *******************************************************************************/
#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"
#include "cbuf.h"
void USARTInit(USART_TypeDef* USARTX, uint32_t BaudRate, char enable_rxit);
void USARTSendBytes(USART_TypeDef *usart,const unsigned char *ch,int count);
void USARTSendByte(USART_TypeDef *usart,char c);
void USARTPrint(USART_TypeDef *usart,char *str);
void USARTPrintln(USART_TypeDef *usart,u8 *str);
void USARTPrintf(USART_TypeDef *usart,char *fmt, ...);
unsigned char USARTRecvChar(USART_TypeDef *usart);
int USARTScanf(USART_TypeDef *usart,const char *fmt, ...);//End of line： 0x0d,0x0a

#endif




