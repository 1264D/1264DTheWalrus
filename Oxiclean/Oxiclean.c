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
#pragma config(Motor,  port1,            ,             tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           FrontRight,    tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port3,           BackRight,     tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_2)
#pragma config(Motor,  port4,            ,             tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           Arm1,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           Arm2,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,            ,             tmotorVex393_MC29, openLoop)
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

int ArmAngle; //Preset angles
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

int PowerCap(int motorPower){ //intake joystick position
	if(abs(motorPower) <= 127){ //if power value is between 127 & -127 then return actual power
		return motorPower;
	}
	else if(motorPower > 127){ //if above 127, set to 127
		return 127;
	}
	else{ //if below -127, set to -127
		return -127;
	}
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
		motor[Arm2] = 0;
	}
	else if((RealAngle <= (SensorValue[Poten1] - 40))){ //If Poten2 is lower then Poten1 then raise Poten2
		AngleToggle2 = true;
		motor[Arm2] = 66;
	}
	else if((RealAngle >= (SensorValue[Poten1] + 40))){ //If Poten2 is higher then Poten1 then lower Poten2
		AngleToggle2 = true;
		motor[Arm2] = -66;
	}
}

void AngleArm(){
	if(AngleToggle == true){
		if((SensorValue[Poten1] >= ArmAngle - 40) && (SensorValue[Poten1] <= ArmAngle + 40)) { //If Potent1 matches the requested angle then finish
			motor[Arm1] = 0;
			motor[Arm2] = 0;
			AngleToggle = false;
		}
		if(SensorValue[Poten1] <= ArmAngle - 40){ //If Poten1 is less than the request angle, raise lift
			motor[Arm1] = 127;
			motor[Arm2] = 127;
		}
		if(SensorValue[Poten1] >= ArmAngle + 40){ //If Poten1 is higher than the requested angle, lower lift
			motor[Arm1] = -127;
			motor[Arm2] = -127;
		}
	}
}

void Base(){
	if(SensorValue[ArcadeContol] == 1){
		motor[FrontLeft] = PowerCap(RightJoyMV + RightJoyMH + vexRT[Btn5U]*120 + vexRT[Btn6U]*-120 + vexRT[Btn5D]*40 + vexRT[Btn6D]*-40);
		motor[FrontRight] = PowerCap(RightJoyMV + RightJoyMH + vexRT[Btn5U]*-120 + vexRT[Btn6U]*120 + vexRT[Btn5D]*-40 + vexRT[Btn6D]*40);
		motor[BackLeft] = PowerCap(RightJoyMV + RightJoyMH + vexRT[Btn5U]*120 + vexRT[Btn6U]*-120 + vexRT[Btn5D]*-40 + vexRT[Btn6D]*40);
		motor[BackRight] = PowerCap(RightJoyMV + RightJoyMH + vexRT[Btn5U]*-120 + vexRT[Btn6U]*120 + vexRT[Btn5D]*40 + vexRT[Btn6D]*-40);
		//Control Drive using horizontal and vertical axes and upper bumbers for quick turn and lower for percise turn
		//Left bumpers turns counter clockwise and right bumpers turn clockwise
	}
	else{
		motor[FrontLeft] = PowerCap(LeftJoyMV + vexRT[Btn5U]*-100 + vexRT[Btn6U]*100);
		//Control front left wheel using left main joystick and strafe left and strafe right using upper bumpers
		motor[FrontRight] = PowerCap(RightJoyMV + vexRT[Btn5U]*100 + vexRT[Btn6U]*-100);
		//Control front right wheel using right main joystick and strafe left and right using upper bumpers
		motor[BackLeft] = PowerCap(LeftJoyMV + vexRT[Btn5U]*100 + vexRT[Btn6U]*-100);
		//Control back left wheel using left main joystick and strafe left and right using upper bumpers
		motor[BackRight] = PowerCap(RightJoyMV + vexRT[Btn5U]*-100 + vexRT[Btn6U]*100);
		//Control back right wheel using right main joystick and strafe left and right using upper bumpers
	}
}

void Lift(){
	if(SensorValue[TwoRemote] == 1){ //If Two Remote jumper is in
		if(vexRT[Btn8LXmtr2] == 1){ //Btn8U on second remote requests angle to top
			ArmAngle = 0; //Insert Hang Value
			AngleToggle = true;
		}
		else if(vexRT[Btn8UXmtr2] == 1){ //Btn8U on second remote requests angle to Up
			ArmAngle = 3270; //Change
			AngleToggle = true;
		}
		else if(vexRT[Btn8DXmtr2] == 1){ //Btn8D on second remote requests angle to bottom
			ArmAngle = 1820;
			AngleToggle = true;
		}
	}
	else if(SensorValue[ArcadeContol] == 1){ //If 1 person mode is enabled and Arcade control is enabled
		if(vexRT[Btn7L] == 1){
			ArmAngle = 0; //Insert Hang Value
			AngleToggle = true;
		}
		else if(vexRT[Btn7U] == 1){ //Btn8U brings lift up
			ArmAngle = 3270; //Change
			AngleToggle = true;
		}
		else if(vexRT[Btn7D] == 1){ //Btn8D brings lift down
			ArmAngle = 1820;
			AngleToggle = true;
		}
	}
	else{
		if(vexRT[Btn7R] == 1){
			ArmAngle = 0; //Insert Hang Value
			AngleToggle = true;
		}
		else if(vexRT[Btn8U] == 1){ //Btn8U brings lift up
			ArmAngle = 3270; //Change
			AngleToggle = true;
		}
		else if(vexRT[Btn8D] == 1){ //Btn8D brings lift down
			ArmAngle = 1820;
			AngleToggle = true;
		}
	}
	AngleCorrect();
	AngleArm();
	if(AngleToggle == false && SensorValue[TwoRemote] == 1 && AngleToggle2 == false){ //If it is not correcting or moving to presets and two person mode is enabled
		motor[Arm1] = RightJoySV; //Control lift with 2nd remote right joystick
		motor[Arm2] = RightJoySV;
	}
	else if(AngleToggle == false && AngleToggle == false){//If it is not correcting or moving to preset and one person mode is enabled
		if(SensorValue[ArcadeContol] == 1){
			motor[Arm1] = LeftJoyMV;
			motor[Arm2] = LeftJoyMV;
		}
		else{
			motor[Arm1]= PowerCap(vexRT[Btn6D]*127 + vexRT[Btn5D]*-127); //Control lift with Btns 8U(Up) and 8D(Down)
			motor[Arm2]= PowerCap(vexRT[Btn6D]*127 + vexRT[Btn5D]*-127);
		}
	}
}

void Control(){
	Base();
	Lift();
}

void pre_auton()
{
	bStopTasksBetweenModes = true;
}

task autonomous()
{
	AutonomousCodePlaceholderForTesting();
}

task usercontrol()
{

	while (true)
	{
		Control();
		Variables();
	}
}
