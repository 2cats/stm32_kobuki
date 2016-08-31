/*******************************************************************************
 * Copyright (C) 2016 Ence Wang
 *
 * MIT License
 *******************************************************************************/
#include "sys.h"
#include <stdarg.h>
#include <usart.h>
#define CMD_BUFFER_LEN  			200
void USARTInit(USART_TypeDef* USARTX, uint32_t BaudRate, char enable_rxit) {
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*USART Function Configuration*/
	if (USARTX == USART1) {
		//Enable the Clock for USART
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	} else if (USARTX == USART2) {
		//Enable the Clock for USART
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	} else {
		//Enable the Clock for USART
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	}
	//Enable AFIO Clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	//Parameters for USART
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// Configure USART
	USART_Init(USARTX, &USART_InitStructure);

	//Enable
	USART_Cmd(USARTX, ENABLE);
	while (USART_GetFlagStatus(USARTX, USART_FLAG_TC) == RESET)
		;

	/*USART Tx/Rx Pin Configuration*/
	//Enable USART Tx/Rx Pin Clock
	if ((USARTX == USART1) || (USARTX == USART2)) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	} else if (USARTX == USART3) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	}

	/*USART Tx/Rx Pin*/
	if (USARTX == USART1) {
		//Parameters for Rx Pin
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		//Parameters for Tx Pin
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	} else if (USARTX == USART2) {
		//Parameters for Rx Pin
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //GPIO_Pin_2;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		//Parameters for Tx Pin
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIO_Pin_3;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	} else if (USARTX == USART3) {
		//Parameters for Rx Pin
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //GPIO_Pin_10;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		//Parameters for Tx Pin
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //GPIO_Pin_9;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}

	if (enable_rxit) {
		USART_ITConfig(USARTX, USART_IT_RXNE, ENABLE);
		/*Interrupt Configuration*/
		if (USARTX == USART1) {
			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		} else if (USARTX == USART2) {
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		} else if (USARTX == USART3) {
			NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		}
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
}
void USARTSendBytes(USART_TypeDef *usart, const unsigned char *ch, int count) {
	int i = 0;
	while (i < count) {
		while (USART_GetFlagStatus(usart, USART_FLAG_TC) == RESET)
			;
		USART_SendData(usart, (uint8_t) ch[i]);
		i++;
	}
	return;
}
void USARTSendByte(USART_TypeDef *usart, char c) {
	while (USART_GetFlagStatus(usart, USART_FLAG_TC) == RESET)
		;
	USART_SendData(usart, (uint8_t) c);
	return;
}

void USARTPrint(USART_TypeDef *usart, char *str) {
	int i = 0;
	while (str[i] != '\0') {
		USARTSendByte(usart, str[i++]);
	}
}
void USARTPrintln(USART_TypeDef *usart, u8 *str) {
	int i = 0;
	while (str[i] != '\0') {
		USARTSendByte(usart, str[i++]);
	}
	USARTSendByte(usart, '\n');
}
void USARTPrintf(USART_TypeDef *usart, char *fmt, ...) {
	char buffer[CMD_BUFFER_LEN + 1];
	u8 i = 0;
	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buffer, CMD_BUFFER_LEN + 1, fmt, arg_ptr);
	while ((i < CMD_BUFFER_LEN) && buffer[i]) {
		USARTSendByte(usart, buffer[i++]);
	}
	va_end(arg_ptr);
}
unsigned char USARTRecvChar(USART_TypeDef *usart) {
	while (USART_GetFlagStatus(usart, USART_FLAG_RXNE) == RESET)
		;
	return usart->DR;
}
//End of line： 0x0d,0x0a
int USARTScanf(USART_TypeDef *usart, const char *fmt, ...) {
	char buffer[CMD_BUFFER_LEN + 1];
	va_list arg_ptr;
	int i = 0;
	unsigned char c;
	while (1) {
		c = USARTRecvChar(usart);
		if ((c == 0x0d) || (c == 0x0a)) {
			buffer[i] = '\0';
			break;
		} else {
			buffer[i++] = c;
		}
	}
	va_start(arg_ptr, fmt);
	i = vsscanf(buffer, fmt, arg_ptr);
	va_end(arg_ptr);
	return i;
}

void __attribute__((weak)) OnUSART1RecvByte(unsigned char ch) {
}
void __attribute__((weak)) OnUSART2RecvByte(unsigned char ch) {
}
void __attribute__((weak)) OnUSART3RecvByte(unsigned char ch) {
}
void USART1_IRQHandler(void) {
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
		OnUSART2RecvByte(USART1->DR);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
void USART2_IRQHandler(void) {
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
		OnUSART2RecvByte(USART2->DR);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}
void USART3_IRQHandler(void) {
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET) {
		OnUSART3RecvByte(USART3->DR);
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}

