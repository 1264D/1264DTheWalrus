#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    Poten1,         sensorPotentiometer)
#pragma config(Sensor, in2,    Poten2,         sensorPotentiometer)
#pragma config(Sensor, dgtl1,  TwoRemote,      sensorTouch)
#pragma config(Sensor, dgtl2,  ArcadeContol,   sensorTouch)
#pragma config(Sensor, dgtl3,  ,               sensorTouch)
#pragma config(Sensor, dgtl4,  ,               sensorTouch)
#pragma config(Sensor, dgtl5,  ,               sensorTouch)
#pragma config(Sensor, dgtl6,  ,               sensorTouch)
#pragma config(Sensor, dgtl7,  ,               sensorTouch)
#pragma config(Sensor, dgtl8,  ,               sensorTouch)
#pragma config(Sensor, dgtl9,  ,               sensorTouch)
#pragma config(Sensor, dgtl10, ,               sensorTouch)
#pragma config(Sensor, dgtl11, NearvsFar,      sensorTouch)
#pragma config(Sensor, dgtl12, BluevsRed,      sensorTouch)
#pragma config(Sensor, I2C_1,  FrontLeft,      sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  FrontRight,     sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  BackLeft,       sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_4,  BackRight,      sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_5,  Manip,          sensorNone)
#pragma config(Motor,  port1,            ,             tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           FrontRight,    tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port3,           BackRight,     tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_2)
#pragma config(Motor,  port4,           Manipulator1,  tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           Lift1,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           Lift2,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           Manipulator2,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           FrontLeft,     tmotorVex393_MC29, openLoop, encoderPort, I2C_3)
#pragma config(Motor,  port9,           BackLeft,      tmotorVex393_MC29, openLoop, encoderPort, I2C_4)
#pragma config(Motor,  port10,           ,             tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

int LiftAngle; //Preset angles
bool AngleToggle; //Disables lift while moving to presets
bool AngleToggle2; //Disables lift while correcting
int RealAngle; //Converts 2nd Potentiometer value to 1st
int RightJoyMV; //Main Right Y
int RightJoySV; //Partner Right Y
int LeftJoyMV;  //Main Left Y
int LeftJoySV;  //Partner Left Y
int LeftJoyMH; //Main Left X
int LeftJoySH; //Partner Left X
int RightJoyMH; //Main Right X
int RightJoySH; //Partner Right X

int JoyClear(int origVal) { //intake current joystick position
	if(abs(origVal) < 10){ // if joystick is close to still just return 0
		return 0;
	}
	else{
		return origVal; //else, function properly
	}
	//Stops the idle motor sound
}

void Variables(){
	RightJoyMV = JoyClear(vexRT[Ch2]);
	RightJoySV = JoyClear(vexRT[Ch2Xmtr2]);
	LeftJoyMV = JoyClear(vexRT[Ch3]);
	LeftJoySV = JoyClear(vexRT[Ch3Xmtr2]);
	RightJoyMH = JoyClear(vexRT[Ch1]);
	RightJoySH = JoyClear(vexRT[Ch1Xmtr2]);
	LeftJoyMH = JoyClear(vexRT[Ch4]);
	LeftJoySH = JoyClear(vexRT[Ch4Xmtr2]);
	//Configure joysticks for deidling
}
void AngleCorrect(){
	RealAngle = SensorValue[Poten2] + 700; //convertpoteniometer2
	if((RealAngle >= (SensorValue[Poten1] - 40)) && (RealAngle <= (SensorValue[Poten1] + 40)) && AngleToggle2 == true){ //If the Poten1 matches Poten2 then disable
		AngleToggle2 = false;
		motor[Lift1] = 0;
	}
	else if((RealAngle <= (SensorValue[Poten1] - 40))){ //If Poten2 is lower then Poten1 then raise Poten2
		AngleToggle2 = true;
		motor[Lift1] = 66;
	}
	else if((RealAngle >= (SensorValue[Poten1] + 40))){ //If Poten2 is higher then Poten1 then lower Poten2
		AngleToggle2 = true;
		motor[Lift1] = -66;
	}
}

void AngleLift(){
	if(AngleToggle == true){
		if((SensorValue[Poten1] >= LiftAngle - 40) && (SensorValue[Poten1] <= LiftAngle + 40)) { //If Potent1 matches the requested angle then finish
			motor[Lift1] = 0;
			motor[Lift2] = 0;
			AngleToggle = false;
		}
		if(SensorValue[Poten1] <= LiftAngle - 40){ //If Poten1 is less than the request angle, raise lift
			motor[Lift1] = 127;
			motor[Lift2] = 127;
		}
		if(SensorValue[Poten1] >= LiftAngle + 40){ //If Poten1 is higher than the requested angle, lower lift
			motor[Lift1] = -127;
			motor[Lift2] = -127;
		}
	}
}

void Base(){
	if(SensorValue[ArcadeContol] == 1){
		motor[FrontLeft] = RightJoyMV + RightJoyMH;
		motor[FrontRight] = RightJoyMV + RightJoyMH;
		motor[BackLeft] = RightJoyMV + RightJoyMH;
		motor[BackRight] = RightJoyMV + RightJoyMH;
	}
	else{
		motor[FrontLeft] = LeftJoyMV + vexRT[Btn5U]*-75 + vexRT[Btn6U]*75; //Control front left wheel using left main joystick and strafe left and strafe right using upper bumpers
		motor[FrontRight] = RightJoyMV + vexRT[Btn5U]*75 + vexRT[Btn6U]*-75; //Control front right wheel using right main joystick and strafe left and right using upper bumpers
		motor[BackLeft] = LeftJoyMV + vexRT[Btn5U]*75 + vexRT[Btn6U]*-75; //Control back left wheel using left main joystick and strafe left and right using upper bumpers
		motor[BackRight] = RightJoyMV + vexRT[Btn5U]*-75 + vexRT[Btn6U]*75; //Control back right wheel using right main joystick and strafe left and right using upper bumpers
	}
}

void Lift(){
	if(SensorValue[TwoRemote] == 1){ //If Two Remote jumper is in
		if(vexRT[Btn8UXmtr2] == 1){ //Btn8U on second remote requests angle to top
			LiftAngle = 3270;
			AngleToggle = true;
		}
		else if(vexRT[Btn8DXmtr2] == 1){ //Btn8D on second remote requests angle to bottom
			LiftAngle = 1820;
			AngleToggle = true;
		}
	}
	else{ //Jumper is not there, ie: if 1 person mode is enabled
		if(vexRT[Btn8L] == 1){ //Btn8L brings lift up
			LiftAngle = 3270;
			AngleToggle = true;
		}
		else if(vexRT[Btn8R] == 1){ //Btn8R brings lift down
			LiftAngle = 1820;
			AngleToggle = true;
		}
	}
	AngleCorrect();
	AngleLift();
	if(AngleToggle == false && SensorValue[TwoRemote] == 1 && AngleToggle2 == false){ //If it is not correcting or moving to presets and two person mode is enabled
		motor[Lift1]= RightJoySV; //Control lift with 2nd remote right joystick
		motor[Lift2]= RightJoySV;
	}
	else if(AngleToggle == false && AngleToggle == false){//If it is not correcting or moving to preset and one person mode is enabled
		if(SensorValue[ArcadeContol] == 1){
			motor[Lift1] = LeftJoyMV;
			motor[Lift2] = LeftJoyMV;
		}
		else{
			motor[Lift1]= vexRT[Btn8U]*127 + vexRT[Btn8D]*-127; //Control lift with Btns 8U(Up) and 8D(Down)
			motor[Lift2]= vexRT[Btn8U]*127 + vexRT[Btn8D]*-127;
		}
	}
}

void Manipulator(){
	if(SensorValue[TwoRemote] == 1){ //If two person mode is enabled
		motor[Manipulator1] = LeftJoySV; //ontrol manipulator with second remote left joystick
		motor[Manipulator2] = LeftJoySV;
	}
	else{//If one person mode is enabled
		motor[Manipulator1] = vexRT[Btn6D]*127 + vexRT[Btn5D]*-127; //Control manipulator with lower bumpers
		motor[Manipulator2] = vexRT[Btn6D]*127 + vexRT[Btn5D]*-127;
	}
}

void Control(){
	Base();
	Lift();
	Manipulator();
}

void pre_auton(){
	bStopTasksBetweenModes = true;
}

task autonomous(){
	AutonomousCodePlaceholderForTesting();
}

task usercontrol(){
	while (true){
		Control();
		Variables();
	}
}
