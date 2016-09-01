/*******************************************************************************
 * Copyright (C) 2016 Ence Wang
 *
 * MIT License
 *******************************************************************************/
#include <tools.h>
#include "kobuki.h"
#include "motor.h"
#define MMS2RPM(mms) (mms*100.0*60/(2*PI)/(WHEEL_DIAMETER/2))

ControllerInfo PIDConf;
int lSpeed,rSpeed;
void OnBaseControl(BaseControl *data) {
	SIGNED_BYTE_2 radius=(SIGNED_BYTE_2)(data->radius);
	SIGNED_BYTE_2 speed=(SIGNED_BYTE_2)(data->speed);
	//LOG("r:%d,s:%d",radius,speed);
	if(0==radius){
		rSpeed=lSpeed=speed;
	}else if(radius<=1&&radius>=-1)
	{
		int sign=speed>0?1:-1;
		rSpeed=sign*ROTATION_SPEED;
		lSpeed=-rSpeed;
	}else{
		SIGNED_BYTE_2 radius_abs=radius>0?radius:-radius;
		int sign=radius>0?1:-1;
		rSpeed= speed*(radius_abs+sign*WHEELBASE_LENGTH/2)/radius_abs;
		lSpeed= speed*(radius_abs-sign*WHEELBASE_LENGTH/2)/radius_abs;
	}
	LOG("##r:%d,l:%d",rSpeed,lSpeed);
	rSpeed=MMS2RPM(rSpeed);
	lSpeed=-MMS2RPM(lSpeed);
	LOG("r:%d,l:%d",rSpeed,lSpeed);
	USARTPrintf(leftMotor.usart,"V%d\x0d",lSpeed);
	USARTPrintf(rightMotor.usart,"V%d\x0d",rSpeed);
}
void OnRequestExtra(RequestExtra *data) {
	FirmwareVersion fv;
	HardwareVersion hv;
	UniqueDeviceIDentifier uuid;
	//LOG("OnRequestExtra");
	if (BIT_MASK(data->requestFlags, FLAG_REQUEST_HARDWARE_VERSION)) {
		ResetUpload();
		hv.major = HARDWARE_VERSION_MAJOR;
		hv.minor = HARDWARE_VERSION_MINOR;
		hv.patch = HARDWARE_VERSION_PATCH;
		AddHardwareVersion(&hv);
		Upload();

	}
	if (BIT_MASK(data->requestFlags, FLAG_REQUEST_FIRMWARE_VERSION)) {
		ResetUpload();
		fv.major = FIRMWARE_VERSION_MAJOR;
		fv.minor = FIRMWARE_VERSION_MINOR;
		fv.patch = FIRMWARE_VERSION_PATCH;
		AddFirmwareVersion(&fv);
		Upload();

	}
	if (BIT_MASK(data->requestFlags, FLAG_REQUEST_UUID)) {
		ResetUpload();
		uuid = GetUUID();
		AddUniqueDeviceIDentifier(&uuid);
		Upload();
	}
}
void OnGeneralPurposeOutput(GeneralPurposeOutput *data) {
	unsigned char led1 =
			BIT_MASK(data->digitalOutputFlags,
					FLAG_SET_LED1_RED)||BIT_MASK(data->digitalOutputFlags,FLAG_SET_LED1_GREEN);
	unsigned char led2 =
			BIT_MASK(data->digitalOutputFlags,
					FLAG_SET_LED2_RED)||BIT_MASK(data->digitalOutputFlags,FLAG_SET_LED2_GREEN);
	LOG("OnGeneralPurposeOutput");
	LOG("led1:%d\n", led1);
	LOG("led2:%d\n", led2);
}
void OnSetControllerGain(SetControllerGain *data) {
	//LOG("OnSetControllerGain");
	PIDConf.pGain = data->pGain;
	PIDConf.iGain = data->iGain;
	PIDConf.dGain = data->dGain;
}
void OnGetControllerGain(GetControllerGain *data) {
	//LOG("OnGetControllerGain");
	ResetUpload();
	AddControllerInfo(&PIDConf);
	Upload();
}
void OnSound(Sound* data){
	//LOG("OnSound");
}
void OnSoundSequence(SoundSequence* data){
	//LOG("OnSoundSequence");
}

