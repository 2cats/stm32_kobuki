#ifndef __USART23BR_H
#define __USART23BR_H
#include <cbuf.h>
extern CBuf leftBuf;
extern CBuf rightBuf;
#define LEFT_USART		USART2
#define RIGHT_USART		USART3
void Uart23brInit(unsigned int bound);
void MotorInit(unsigned int bound);
#endif
