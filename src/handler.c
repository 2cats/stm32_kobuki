#include "handler.h"
#define ROTATION_SPEED		100
#define WHEELBASE_LENGTH	230
#define CURRENT_FIRMWARE_MAJOR_VERSION  1
#define CURRENT_FIRMWARE_MINOR_VERSION  2
ControllerInfo PIDConf;
extern int lSpeed,rSpeed;
void OnBaseControl(BaseControl *data) {
	//do control
	SIGNED_BYTE_2 radius=(SIGNED_BYTE_2)(data->radius);
	SIGNED_BYTE_2 speed=(SIGNED_BYTE_2)(data->speed);
	if(0==radius){
		rSpeed=lSpeed=speed;
	}else if(radius<=1&&radius>=-1)
	{
		SIGNED_BYTE_1 sign=speed>0?1:-1;
		rSpeed=sign*ROTATION_SPEED;
		lSpeed=-rSpeed;
	}else{
		SIGNED_BYTE_2 radius_abs=radius>0?radius:-radius;
		SIGNED_BYTE_1 sign=radius*speed>0?1:-1;
		rSpeed= speed*(radius_abs+sign*WHEELBASE_LENGTH/2)/radius_abs;
		lSpeed= speed*(radius_abs-sign*WHEELBASE_LENGTH/2)/radius_abs;
	}
}
void OnRequestExtra(RequestExtra *data) {
	LOG("OnRequestExtra");
	FirmwareVersion fv;
	HardwareVersion hv;
	UniqueDeviceIDentifier uuid;
	if (BIT_MASK(data->requestFlags, FLAG_REQUEST_HARDWARE_VERSION)) {
		ResetUpload();
		hv.major = 3;
		hv.minor = 1;
		hv.patch = 4;
		AddHardwareVersion(&hv);
		Upload();

	}
	if (BIT_MASK(data->requestFlags, FLAG_REQUEST_FIRMWARE_VERSION)) {
		ResetUpload();
		fv.major = CURRENT_FIRMWARE_MAJOR_VERSION;
		fv.minor = CURRENT_FIRMWARE_MINOR_VERSION;
		fv.patch = 4;
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
	LOG("OnGeneralPurposeOutput");
	unsigned char led1 =
			BIT_MASK(data->digitalOutputFlags,
					FLAG_SET_LED1_RED)||BIT_MASK(data->digitalOutputFlags,FLAG_SET_LED1_GREEN);
	unsigned char led2 =
			BIT_MASK(data->digitalOutputFlags,
					FLAG_SET_LED2_RED)||BIT_MASK(data->digitalOutputFlags,FLAG_SET_LED2_GREEN);
	LOG("led1:%d\n", led1);
	LOG("led2:%d\n", led2);
}
void OnSetControllerGain(SetControllerGain *data) {
	LOG("OnSetControllerGain");
	PIDConf.pGain = data->pGain;
	PIDConf.iGain = data->iGain;
	PIDConf.dGain = data->dGain;
}
void OnGetControllerGain() {
	LOG("OnGetControllerGain");
	ResetUpload();
	AddControllerInfo(&PIDConf);
	Upload();
}
