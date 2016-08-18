#include "kobuki.h"
#include "package.h"
#include "delay.h"
#include "millis.h"
#include "tools.h"
#include "uartbr.h"
#include "test.h"
int main(int argc, char* argv[]) {
	delay_init();
	KobukiInit();
	RandInit();
	while (1) {
		TestFeedbackFilling();
		KobukiProcessing();
	}
}
