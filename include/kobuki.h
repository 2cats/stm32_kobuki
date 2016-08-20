/*
 * Created By Ence Wang , Summer 2016
 * Protocol Specification : http://yujinrobot.github.io/kobuki/doxygen/enAppendixProtocolSpecification.html
 * Changes:
 *	 Current	:	Total -> 2
 *
 */
# ifndef _KOBUKI_H_
# define _KOBUKI_H_
#include <string.h>

#define HEADER_0						0xAA	//Protocol Magic
#define HEADER_1						0x55    //Protocol Magic
#define FIRMWARE_VERSION_MAJOR 1
#define FIRMWARE_VERSION_MINOR 2
#define FIRMWARE_VERSION_PATCH 3

#define HARDWARE_VERSION_MAJOR 3
#define HARDWARE_VERSION_MINOR 1
#define HARDWARE_VERSION_PATCH 4

#define ROTATION_SPEED			100		//Rotation Speed
#define WHEELBASE_LENGTH		230		//Diameter

#define MAX_SUBPAYLOAD_LENGTH			0xff	//Max (all)data size(bytes) of one package
/*
 *	API
 *
 *	1.  Uploading
 *	Example:
 *		FirmwareVersion fv;
 *		ResetUpload();
 *		fv.major = FIRMWARE_VERSION_MAJOR;
 *		fv.minor = FIRMWARE_VERSION_MINOR;
 *		fv.patch = FIRMWARE_VERSION_PATCH;
 *		AddFirmwareVersion(&fv);
 *		Upload();
 *
 *	2. Receiving
 *  Example:
 *  	1. Fill CallBacks
 *		void OnBaseControlHandler(BaseControl *baseControl)
 *		{
 *			//your code here
 *		}
 *		2. Feeding
 *		while(1)
 *		{
 *			KobukiProcessing();
 *		}
 *  *3. Add New A SubPackage Type
 *  Example:
 *		1).[.h] typedef struct {BYTE_1 field0;} EndDataTypeDef(MyType);
 *		2).[.c] DefineAdaper(MyType, 0x31);
 */

#pragma push
#pragma pack (1)								//!This is important(no align) for 'sizeof(DataType)'

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

typedef char SIGNED_BYTE_1;
typedef short SIGNED_BYTE_2;
typedef int SIGNED_BYTE_4;

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
#define EndDataTypeDef(DataType)			DataType;DeclareAdaper(DataType);extern const BYTE_1 HEADER(DataType);void On##DataType(DataType*);
#define DefineAdaper(DataType,Header)		void Add##DataType(DataType *data)\
{\
	int dataLength=sizeof(DataType);\
	__sendPackage.subPayloads[__sendPackage.length++]=HEADER(DataType);\
	__sendPackage.subPayloads[__sendPackage.length++]=dataLength;\
	memcpy(__sendPackage.subPayloads+__sendPackage.length,data,dataLength);\
	__sendPackage.length+=dataLength;\
};\
const BYTE_1 HEADER(DataType)=Header;

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
	BYTE_2 x;
	BYTE_2 y;
	BYTE_2 z;
} Byte2Vector3;
typedef struct {
	BYTE_1 frameId;
	BYTE_1 constant_6;
	Byte2Vector3 rawGyroData[2];
}EndDataTypeDef(RawDataOf3DGyro_2)
;
typedef struct {
	BasicSensorData *p_BasicSensorData;
	DockingIR *p_DockingIR;
	InertialSensorData *p_InertialSensorData;
	CliffSensorData *p_CliffSensorData;
	Current *p_Current;
	GeneralPurposeInput *p_GeneralPurposeInput;
	RawDataOf3DGyro_2 *p_RawDataOf3DGyro_2;
	BYTE_1 __uploadNeeded;
} FeedbackPacketsDef;
extern FeedbackPacketsDef FeedbackPackets;
UniqueDeviceIDentifier GetUUID(void);
void KobukiInit(void);
void ResetUpload(void);
void Upload(void);
void KobukiProcessing(void);
#pragma pop
#endif
