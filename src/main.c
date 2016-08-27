#include "kobuki.h"
#include "delay.h"
#include "millis.h"
#include "uartbr.h"
#include "test.h"
void KobukiDoBeforeUpload(){
	TestFeedbackFilling();
}
int main(int argc, char* argv[]) {
	delay_init();
	KobukiInit();
	while (1) {
		KobukiProcessing();
	}
}
