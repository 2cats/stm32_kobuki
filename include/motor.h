#ifndef __USART23BR_H
#define __USART23BR_H
#include <cbuf.h>
#include <usart.h>
#define MOTOR_LINEBUF_LEN	128
typedef struct{
	CBuf buf;
	unsigned char lineBuf[MOTOR_LINEBUF_LEN];
	USART_TypeDef* usart;
	int lineBuf_i;
	int encoder;
}Motor;
extern Motor leftMotor;
extern Motor rightMotor;
void Uart23brInit(unsigned int bound);
void MotorInit(unsigned int bound);

#endif
