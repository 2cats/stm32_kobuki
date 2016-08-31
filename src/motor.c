/*******************************************************************************
 * Copyright (C) 2016 Ence Wang
 *
 * MIT License
 *******************************************************************************/
#include <usart.h>
#include <motor.h>
Motor leftMotor;
Motor rightMotor;
void OnUSART2RecvByte(unsigned char ch) {
	CBUF_ADDM(leftMotor.buf,ch);
}
void OnUSART3RecvByte(unsigned char ch) {
	CBUF_ADDM(rightMotor.buf,ch);
}
void MotorInit(unsigned int bound)
{
	leftMotor.usart=USART2;
	rightMotor.usart=USART3;
	leftMotor.lineBuf_i=0;
	rightMotor.lineBuf_i=0;
	CBufInit(&leftMotor.buf);
	CBufInit(&rightMotor.buf);
	USARTInit(leftMotor.usart,bound,1);
	USARTInit(rightMotor.usart,bound,1);
	USARTPrint(leftMotor.usart,"ENA\x0d");
	USARTPrint(rightMotor.usart,"ENA\x0d");
}
