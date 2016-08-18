#include "handler.h"
#define CURRENT_FIRMWARE_MAJOR_VERSION  1
#define CURRENT_FIRMWARE_MINOR_VERSION  2
ControllerInfo PIDConf;

void OnBaseControl(BaseControl *data) {
	//do control
	LOG("OnBaseControl:Speed->%d,Radius->%d",data->speed,data->radius);
//	Speed=data->speed;
//	Angle=data->radius;
}
void OnRequestExtra(RequestExtra *data) {
	LOG("OnRequestExtra");
	FirmwareVersion fv;
	HardwareVersion hv;
	UniqueDeviceIDentifier uuid;
	if(BIT_MASK(data->requestFlags,FLAG_REQUEST_HARDWARE_VERSION))
	{
		ResetUpload();
		hv.major = 3;
		hv.minor = 1;
		hv.patch = 4;
		AddHardwareVersion(&hv);
		Upload();

	}
	if(BIT_MASK(data->requestFlags,FLAG_REQUEST_FIRMWARE_VERSION))
	{
		ResetUpload();
		fv.major = CURRENT_FIRMWARE_MAJOR_VERSION;
		fv.minor = CURRENT_FIRMWARE_MINOR_VERSION;
		fv.patch = 4;
		AddFirmwareVersion(&fv);
		Upload();

	}
	if(BIT_MASK(data->requestFlags,FLAG_REQUEST_UUID))
	{
		ResetUpload();
		uuid.uuid0 = 0x07;
		uuid.uuid1 = 0x08;
		uuid.uuid2 = 0x09;
		AddUniqueDeviceIDentifier(&uuid);
		Upload();
	}
}
void OnGeneralPurposeOutput(GeneralPurposeOutput *data) {
	LOG("OnGeneralPurposeOutput");
	unsigned char led1=	BIT_MASK(data->digitalOutputFlags,FLAG_SET_LED1_RED)||BIT_MASK(data->digitalOutputFlags,FLAG_SET_LED1_GREEN);
	unsigned char led2=	BIT_MASK(data->digitalOutputFlags,FLAG_SET_LED2_RED)||BIT_MASK(data->digitalOutputFlags,FLAG_SET_LED2_GREEN);
	LOG("led1:%d\n",led1);
	LOG("led2:%d\n",led2);
}
void OnSetControllerGain(SetControllerGain *data) {
	LOG("OnSetControllerGain");
	PIDConf.pGain=data->pGain;
	PIDConf.iGain=data->iGain;
	PIDConf.dGain=data->dGain;
}
void OnGetControllerGain() {
	LOG("OnGetControllerGain");
	ResetUpload();
	AddControllerInfo(&PIDConf);
	Upload();
}
