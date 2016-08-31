#include <kobuki_feedback.h>
#include "uart1br.h"
#include "kobuki.h"
#include "delay.h"
#include "millis.h"
#include "motor.h"
void KobukiDoBeforeUpload() {
	KobukiFeedbackFilling();
}

void TestMotor()
{
	if(CBufAva(&leftMotor.buf)>0){
		USARTSendByte(USART1,CBufPop(&leftMotor.buf));
	}
//		if(CBufAva(&leftMotor.buf)>0){
//			USARTSendByte(USART1,CBufPop(&leftMotor.buf));
//		}
	if (Uart1brGetAvaliableNum() > 0) {
		unsigned char val = Uart1brGetByte();
		USARTSendByte(leftMotor.usart,val);
	}
}
int main(int argc, char* argv[]) {
	delay_init();
	delay_ms(1000);
	KobukiInit(115200);
	MotorInit(9600);
	while (1) {
		KobukiProcessing();

	}
}
