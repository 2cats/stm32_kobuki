#include "uart1br.h"
#include "kobuki.h"
#include "delay.h"
#include "millis.h"
#include "test.h"
#include "motor.h"
void KobukiDoBeforeUpload(){
	TestFeedbackFilling();
}

int main(int argc, char* argv[]) {
	delay_init();
	KobukiInit();
	MotorInit(115200);
	while (1) {
		if(CBufAva(&leftBuf)>0){
			USARTPrintf(LEFT_USART,"$%c\n",CBufPop(&leftBuf));
		}
		if(CBufAva(&rightBuf)>0){
			USARTPrintf(RIGHT_USART,"$%c\n",CBufPop(&rightBuf));
		}
		KobukiProcessing();
	}
}
