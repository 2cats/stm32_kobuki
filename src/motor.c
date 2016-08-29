#include <usart.h>
CBuf leftBuf;
CBuf rightBuf;
void OnUSART2RecvByte(unsigned char ch) {
	CBUF_ADDM(leftBuf,ch);
}
void OnUSART3RecvByte(unsigned char ch) {
	CBUF_ADDM(rightBuf,ch);
}
void MotorInit(unsigned int bound)
{
	CBufInit(&leftBuf);
	CBufInit(&rightBuf);
	USARTInit(USART2,bound,1);
	USARTInit(USART3,bound,1);
}
