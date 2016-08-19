#include "kobuki.h"
#include "millis.h"
#include "test.h"
BasicSensorData bs;
InertialSensorData is;
CliffSensorData cs;
Current cr;
DockingIR ir;
GeneralPurposeInput gi;
RawDataOf3DGyro_2 g3d;
BYTE_2 Speed=1;
BYTE_2 Angle=1;
void TestFeedbackFilling(void)
{
	bs.timestamp=millis();
	bs.battery=150;
	bs.bumper=0;
	bs.charger=100;
	bs.cliff=0;
	bs.leftEncoder=millis()*Speed;
	bs.rigtEncoder=millis()*Speed;
	bs.overcurrentFlags=200;
	bs.rigtPWM=22;
	bs.leftPWM=33;
	bs.wheelDrop=0;
	is.angle=177;
	is.angleRate=166;
	cr.leftMotor=1;
	cr.rightMotor=1;
	cs.leftCliffSensor=0;
	cs.centralCliffSensor=0;
	cs.rightCliffSensor=0;
	ir.centralSignal=0;
	ir.leftSignal=0;
	ir.rightSignal=0;
	gi.analogInputCh0=100;
	gi.analogInputCh1=111;
	gi.analogInputCh2=122;
	gi.analogInputCh3=133;
	gi.digitalInput=0xffff;
	g3d.constant_6=6;
	g3d.frameId++;
	g3d.rawGyroData[0].x=1344;
	g3d.rawGyroData[0].y=1441;
	g3d.rawGyroData[0].z=1144;
	g3d.rawGyroData[1]=g3d.rawGyroData[0];
	FeedbackPackets.p_BasicSensorData=&bs;
	FeedbackPackets.p_CliffSensorData=&cs;
	FeedbackPackets.p_Current=&cr;
	FeedbackPackets.p_DockingIR=&ir;
	FeedbackPackets.p_GeneralPurposeInput=&gi;
	FeedbackPackets.p_InertialSensorData=&is;
	FeedbackPackets.p_RawDataOf3DGyro_2=&g3d;
}
