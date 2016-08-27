/*******************************************************************************
 * Copyright (C) 2016 Ence Wang
 *
 * MIT License
 *******************************************************************************/

#include "kobuki.h"
#include "usart.h"
#include "millis.h"
#include "uartbr.h"

/*
 * Public Global Variables
 */
FeedbackPacketsDef FeedbackPackets;

/*
 * Private Global Variables
 */
Package __sendPackage;
Package __recvPackage;
/*
 * USART Sender Definition
 */

#define SendBytes 	usart_send_bytes
#define SendByte 	usart_send_byte

/*
 * Command Packets
 */
DefineAdaper(BaseControl, 0x01);
DefineAdaper(Sound, 0x03);
DefineAdaper(SoundSequence, 0x04);
DefineAdaper(RequestExtra, 0x09);
DefineAdaper(GeneralPurposeOutput, 0x0C);
DefineAdaper(SetControllerGain, 0x0D);
DefineAdaper(GetControllerGain, 0x0E);
/*
 * Feedback Packets
 */
DefineAdaper(BasicSensorData, 0x01);
DefineAdaper(DockingIR, 0x03);
DefineAdaper(InertialSensorData, 0x04);
DefineAdaper(CliffSensorData, 0x05);
DefineAdaper(Current, 0x06);
DefineAdaper(HardwareVersion, 10);
DefineAdaper(FirmwareVersion, 11);
DefineAdaper(GeneralPurposeInput, 16);
DefineAdaper(UniqueDeviceIDentifier, 19);
DefineAdaper(ControllerInfo, 21);
DefineAdaper(RawDataOf3DGyro_2,13)
/*
 * Recv Routine State Machine
 */
#define KOBUKI_WAITING_FOR_STX0							0
#define KOBUKI_WAITING_FOR_STX1 						1
#define KOBUKI_WAITING_FOR_PAYLOAD_SIZE					2
#define KOBUKI_WAITING_FOR_PAYLOAD_TO_ETX 				3
#define KOBUKI_WAITING_FOR_ETX							4

BYTE_1 getChecksum(Package*package) {
	BYTE_1 cs = package->length;
	unsigned int i;
	for (i = 0; i < package->length; i++) {
		cs ^= package->subPayloads[i];
	}
	return cs;
}

unsigned char __KobukiProcessing(unsigned char val) {
	static unsigned char state = KOBUKI_WAITING_FOR_STX0;
	static unsigned char size_count = 0;
	unsigned char found_packet = 0;
	switch (state) {
	case KOBUKI_WAITING_FOR_STX0:
		if (val == HEADER_0) {
			state = KOBUKI_WAITING_FOR_STX1;
		}
		break;
	case KOBUKI_WAITING_FOR_STX1:
		if (val == HEADER_1) {
			state = KOBUKI_WAITING_FOR_PAYLOAD_SIZE;
		} else {
			state = KOBUKI_WAITING_FOR_STX0;
		}
		break;
	case KOBUKI_WAITING_FOR_PAYLOAD_SIZE:
		__recvPackage.length = val + 1;
		state = KOBUKI_WAITING_FOR_PAYLOAD_TO_ETX;
		size_count = 0;
		break;
	case KOBUKI_WAITING_FOR_PAYLOAD_TO_ETX:
		if (size_count < __recvPackage.length) {
			__recvPackage.subPayloads[size_count] = val;
			size_count++;
		} else {
			found_packet = 1;
			state = KOBUKI_WAITING_FOR_STX0;
		}
		break;
	default:
		state = KOBUKI_WAITING_FOR_STX0;
		break;
	}
	if (found_packet) {
		__recvPackage.length--;
		__recvPackage.checksum =
				__recvPackage.subPayloads[__recvPackage.length];
		return getChecksum(&__recvPackage) == __recvPackage.checksum;
	} else {
		return 0;
	}
}
#define HandleSubPack(DataType)	\
if(Header##DataType==curPayload->header)\
{\
	On##DataType((DataType*)SUB_PAYLOAD_DATA_PTR(curPayload));\
	break;\
}

#define ADD_FEEDBACK(DataType)	if(FeedbackPackets.p_##DataType!=0){Add##DataType(FeedbackPackets.p_##DataType);FeedbackPackets.__uploadNeeded=1;}
void UploadFeedbackPackets() {
	ResetUpload();
	ADD_FEEDBACK(BasicSensorData);
	ADD_FEEDBACK(DockingIR);
	ADD_FEEDBACK(InertialSensorData);
	ADD_FEEDBACK(CliffSensorData);
	ADD_FEEDBACK(Current);
	ADD_FEEDBACK(GeneralPurposeInput);
	ADD_FEEDBACK(RawDataOf3DGyro_2);
	if (FeedbackPackets.__uploadNeeded)
		Upload();
	memset(&FeedbackPackets, 0, sizeof(FeedbackPackets));
}
void __attribute__ ((weak)) KobukiDoBeforeUpload(void){}

void KobukiProcessing() {
	static unsigned long time = 0;
	if (millis() - time > UPLOAD_INTERVAL) {
		KobukiDoBeforeUpload();
		UploadFeedbackPackets();
		time = millis();
	}
	if (UartbrGetAvaliableNum() > 0) {
		unsigned char val = UartbrGetByte();
		int shift = 0;
		if (__KobukiProcessing(val)) {
			while (1) {
				SubPayload *curPayload =
						(SubPayload *) __recvPackage.subPayloads + shift;
				if (shift + curPayload->length > __recvPackage.length)
					break;
				do {
					HandleSubPack(BaseControl)
					HandleSubPack(Sound);
					HandleSubPack(SoundSequence);
					HandleSubPack(RequestExtra);
					HandleSubPack(GeneralPurposeOutput);
					HandleSubPack(SetControllerGain);
					HandleSubPack(GetControllerGain);
				} while (0);
				shift += curPayload->length + 2;
			}
		}
	}
}
void ResetUpload(void) {
	__sendPackage.length = 0;
}

void Upload() {
	__sendPackage.checksum = getChecksum(&__sendPackage);
	SendByte(HEADER_0);
	SendByte(HEADER_1);
	SendByte(__sendPackage.length);
	SendBytes(__sendPackage.subPayloads, __sendPackage.length);
	SendByte(__sendPackage.checksum);
	ResetUpload();
}
void KobukiInit() {
	MILLIS_Init();
	memset(&FeedbackPackets, 0, sizeof(FeedbackPackets));
	UartbrInit(115200);
}


UniqueDeviceIDentifier GetUUID() {
	UniqueDeviceIDentifier uuid;
	uuid.uuid0 = *(vu32*) (0x1ffff7e8);
	uuid.uuid1 = *(vu32*) (0x1ffff7ec);
	uuid.uuid2 = *(vu32*) (0x1ffff7f0);
	return uuid;
}
