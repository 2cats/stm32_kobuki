/*
 * Created By Ence Wang , Summer 2016
 * Protocol Specification : http://yujinrobot.github.io/kobuki/doxygen/enAppendixProtocolSpecification.html
 *
 */
# ifndef _KOBUKI_H_
# define _KOBUKI_H_
#include <string.h>

/*
 *  Protocol Specification : https://yujinrobot.github.io/kobuki/doxygen/enAppendixProtocolSpecification.html
 *	Protocol Fix:
 *	 Current	:	Total -> 2
 */
/*
 *	API
 *
 *	1. Uploading
 */

void KobukiInit();

void ResetUpload(void);
//BaseControl bs;
//fill bs...
//AddBaseControl(&bs);
void Upload(void);
void KobukiProcessing(void);

/*
 *	2. Receiving
 *  Example:
 *  	1. Register Callback For Each DataType
 *		void OnBaseControlHandler(BaseControl *baseControl)
 *		{
 *			//your code here
 *		}
 *		OnBaseControl=OnBaseControlHandler;
 *		//Other Types ...
 *		2. Feeding
 *		while(1)
 *		{
 *			KobukiProcessing();
 *		}
 *
 *
 *
 */

#pragma push
#pragma pack (1)							//!This is important(no align)

#define HEADER_0					0xAA	//Protocol Magic
#define HEADER_1					0x55    //Protocol Magic
#define MAX_SUBPAYLOAD_LENGTH		0xff	//Max (all)data size(bytes) of one package
#define FLAG_REQUEST_HARDWARE_VERSION	0x01
#define FLAG_REQUEST_FIRMWARE_VERSION	0x02
#define FLAG_REQUEST_UUID				0x08
#define FLAG_SET_LED1_RED				0x0100
#define FLAG_SET_LED1_GREEN				0x0200
#define FLAG_SET_LED2_RED				0x0400
#define FLAG_SET_LED2_GREEN				0x0800


#define BIT_MASK(val,mask)			((val)&(mask))

//#define SETN_BIT(val,nth)			((val)|=(1<<(nth)))
//#define UNSETN_BIT(val,nth)			((val)&=(~(1<<(nth))))


typedef unsigned char BYTE_1;
typedef unsigned short BYTE_2;
typedef unsigned int BYTE_4;

typedef struct {
	BYTE_1 header;
	BYTE_1 length;							//Size of data in bytes
	BYTE_1 *data;
} SubPayload;

typedef struct {
	//Omit the Magic Number
	BYTE_1 length;							//Size of subPayloads in bytes (>=3)
	BYTE_1 subPayloads[MAX_SUBPAYLOAD_LENGTH];
	BYTE_1 checksum;//XOR'ed value of every bytes of bytesStream except headers
} Package;

#define SUB_PAYLOAD_DATA_PTR(subpayload)   	(((unsigned char*)subpayload)+(unsigned int)&(((SubPayload*)0)->data))
#define HEADER(Type)						Header##Type
extern int wec;
//Declare/Define Function Groups
#define DeclareAdaper(DataType)				void Add##DataType(DataType *data);
#define EndDataTypeDef(DataType)			DataType;DeclareAdaper(DataType);extern const BYTE_1 HEADER(DataType);
#define DefineAdaper(DataType,Header)		void Add##DataType(DataType *data)\
{\
	int dataLength=sizeof(DataType);\
	__sendPackage.subPayloads[__sendPackage.length++]=HEADER(DataType);\
	__sendPackage.subPayloads[__sendPackage.length++]=dataLength;\
	memcpy(__sendPackage.subPayloads+__sendPackage.length,data,dataLength);\
	__sendPackage.length+=dataLength;\
};\
const BYTE_1 HEADER(DataType)=Header;\
void __attribute__((weak)) On##DataType(){}

typedef struct {
	BYTE_2 speed;
	BYTE_2 radius;
}EndDataTypeDef(BaseControl)
;

typedef struct {
	BYTE_2 note;
	BYTE_1 duration;
}EndDataTypeDef(Sound)
;

typedef struct {
	BYTE_1 sequenceNumber;
}EndDataTypeDef(SoundSequence)
;

typedef struct {
	BYTE_2 requestFlags;
}EndDataTypeDef(RequestExtra)
;

typedef struct {
	BYTE_2 digitalOutputFlags;
}EndDataTypeDef(GeneralPurposeOutput)
;

typedef struct {
	BYTE_1 type;
	BYTE_4 pGain;
	BYTE_4 iGain;
	BYTE_4 dGain;
}EndDataTypeDef(SetControllerGain)
;

typedef struct {
	BYTE_1 unused;
}EndDataTypeDef(GetControllerGain)
;
typedef struct {
	BYTE_2 timestamp;
	BYTE_1 bumper;
	BYTE_1 wheelDrop;
	BYTE_1 cliff;
	BYTE_2 leftEncoder;
	BYTE_2 rigtEncoder;
	BYTE_1 leftPWM;
	BYTE_1 rigtPWM;
	BYTE_1 button;
	BYTE_1 charger;
	BYTE_1 battery;
	BYTE_1 overcurrentFlags;
}EndDataTypeDef(BasicSensorData)
;
typedef struct {
	BYTE_1 rightSignal;
	BYTE_1 centralSignal;
	BYTE_1 leftSignal;
}EndDataTypeDef(DockingIR)
;
typedef struct {
	BYTE_2 angle;
	BYTE_2 angleRate;
	BYTE_1 unused0;
	BYTE_1 unused1;
	BYTE_1 unused2;
}EndDataTypeDef(InertialSensorData)
;
typedef struct {
	BYTE_2 rightCliffSensor;
	BYTE_2 centralCliffSensor;
	BYTE_2 leftCliffSensor;
}EndDataTypeDef(CliffSensorData)
;
typedef struct {
	BYTE_1 leftMotor;
	BYTE_1 rightMotor;
}EndDataTypeDef(Current)
;
typedef struct {
	BYTE_1 patch;
	BYTE_1 minor;
	BYTE_1 major;
	BYTE_1 unused;
}EndDataTypeDef(HardwareVersion)
;
typedef struct {
	BYTE_1 patch;
	BYTE_1 minor;
	BYTE_1 major;
	BYTE_1 unused;
}EndDataTypeDef(FirmwareVersion)
;
//typedef struct {
//	BYTE_1 patch;
//	BYTE_1 minor;
//	BYTE_1 major;
//	BYTE_1 unused;
//}EndDataTypeDef(RawDataOf3DGyro);
typedef struct {
	BYTE_2 digitalInput;
	BYTE_2 analogInputCh0;
	BYTE_2 analogInputCh1;
	BYTE_2 analogInputCh2;
	BYTE_2 analogInputCh3;
	BYTE_2 unused0;
	BYTE_2 unused1;
	BYTE_2 unused2;
}EndDataTypeDef(GeneralPurposeInput)
;
typedef struct {
	BYTE_4 uuid0;
	BYTE_4 uuid1;
	BYTE_4 uuid2;
}EndDataTypeDef(UniqueDeviceIDentifier)
;
typedef struct {
	BYTE_1 type;
	BYTE_4 pGain;
	BYTE_4 iGain;
	BYTE_4 dGain;
}EndDataTypeDef(ControllerInfo)
;

typedef struct {
	BasicSensorData *p_BasicSensorData;
	DockingIR *p_DockingIR;
	InertialSensorData *p_InertialSensorData;
	CliffSensorData *p_CliffSensorData;
	Current *p_Current;
	GeneralPurposeInput *p_GeneralPurposeInput;
	BYTE_1 __uploadNeeded;
} FeedbackPacketsDef;
extern FeedbackPacketsDef FeedbackPackets;
#pragma pop
#endif
