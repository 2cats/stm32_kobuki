/*******************************************************************************
 * Copyright (C) 2016 Ence Wang
 *
 * MIT License
 *******************************************************************************/
#include "kobuki.h"
#include "millis.h"
#include "kobuki_feedback.h"
#include <motor.h>
#include <tools.h>
BasicSensorData bs;
InertialSensorData is;
CliffSensorData cs;
Current cr;
DockingIR ir;
GeneralPurposeInput gi;
RawDataOf3DGyro_2 g3d;
extern int lSpeed, rSpeed;
int CallMotorWithTimeout(Motor*motor, const char *cmd, char *buf, int buf_len,
		unsigned long timeout) {
	unsigned long time = millis();
	int ret_len_i = 0;
	CBufClear(&motor->buf);
	USARTSendBytes(motor->usart, (const unsigned char *) cmd, strlen(cmd));
	while (1) {
		if (millis() - time > timeout || ret_len_i >= buf_len)
			return 0;
		if (CBufAva(&motor->buf) > 0) {
			unsigned char tmp = CBufPop(&motor->buf);
			if (tmp == 0x0d) {
				buf[ret_len_i] = '\0';
				break;
			}
			buf[ret_len_i++] = tmp;
		}
	}
	return ret_len_i;
}
void updateMotorLine(Motor *motor) {
	int i;
	USARTSendBytes(motor->usart, (const unsigned char*) "gm\x0d", 3);
	//gi.analogInputCh0++;
	while (CBufAva(&motor->buf) > 0) {
		unsigned char tmp = CBufPop(&motor->buf);
		if (tmp == 0x0d) {
			motor->lineBuf[motor->lineBuf_i] = '\0';
			for (i = 0; i < motor->lineBuf_i; i++) {
				if (motor->lineBuf[i] == '#') {
					motor->encoder = atollmod((char*) motor->lineBuf + i + 1,
							6553600) / 100;
					if (motor == &leftMotor) {
						motor->encoder = 65536 - motor->encoder;
					}
					//gi.analogInputCh2++;
					break;
				}
			}
		} else {
			motor->lineBuf[motor->lineBuf_i++] = tmp;
		}

		if (motor->lineBuf_i == MOTOR_LINEBUF_LEN || tmp == 0x0d) {
			motor->lineBuf_i = 0;
		}
//		if (motor->lineBuf_i == MOTOR_LINEBUF_LEN) {
//			gi.analogInputCh1++;
//		}
//		if (tmp == 0x0d) {
//			gi.analogInputCh3++;
//		}
	}
}

void KobukiFeedbackFilling(void) {
	bs.timestamp = millis();
	bs.battery = 150;
	bs.bumper = 0;
	bs.charger = 100;
	bs.cliff = 0;
	updateMotorLine(&leftMotor);
	updateMotorLine(&rightMotor);
	bs.leftEncoder = leftMotor.encoder;
	bs.rigtEncoder = rightMotor.encoder;
	bs.overcurrentFlags = 200;
	bs.rigtPWM = (char) (rSpeed / 100.0);
	bs.leftPWM = (char) (-lSpeed / 100.0);
	bs.wheelDrop = 0;
	is.angle = 0;
	is.angleRate = 1;
	cr.leftMotor = 1;
	cr.rightMotor = 1;
	cs.leftCliffSensor = 0;
	cs.centralCliffSensor = 0;
	cs.rightCliffSensor = 0;
	ir.centralSignal = 0;
	ir.leftSignal = 0;
	ir.rightSignal = 0;
//	gi.analogInputCh0=100;
//	gi.analogInputCh1=111;
//	gi.analogInputCh2=122;
//	gi.analogInputCh3=133;
	gi.digitalInput = 0xffff;
	g3d.constant_6 = 6;
	g3d.frameId++;
	g3d.rawGyroData[0].x = 1344;
	g3d.rawGyroData[0].y = 1441;
	g3d.rawGyroData[0].z = 1144;
	g3d.rawGyroData[1] = g3d.rawGyroData[0];
	FeedbackPackets.p_BasicSensorData = &bs;
	FeedbackPackets.p_CliffSensorData = &cs;
	FeedbackPackets.p_Current = &cr;
	FeedbackPackets.p_DockingIR = &ir;
	FeedbackPackets.p_GeneralPurposeInput = &gi;
	FeedbackPackets.p_InertialSensorData = &is;
	FeedbackPackets.p_RawDataOf3DGyro_2 = &g3d;
}
