#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    Poten1,         sensorPotentiometer)
#pragma config(Sensor, in2,    Poten2,         sensorPotentiometer)
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

int LiftAngle; //Requested angle
bool AngleToggle;
int RealAngle;
int RightJoyM; //Main Right Y
int RightJoyS; //Partner Right Y
int LeftJoyM;  //Main Left Y
int LeftJoyS;  //Partner Left Y

int JoyClear(int origVal) {
	if (abs(origVal) < 10)
		return 0;
	else
		return origVal;
}

void Variables(){
	RightJoyM = JoyClear(vexRT[Ch2]);
	RightJoyS = JoyClear(vexRT[Ch2Xmtr2]);
	LeftJoyM = JoyClear(vexRT[Ch3]);
	LeftJoyS = JoyClear(vexRT[Ch3Xmtr2]);
}
void AngleCorrect(){
	RealAngle = SensorValue[in2] + 700;
	if((RealAngle >= (SensorValue[in1] - 40)) && (RealAngle <= (SensorValue[in1] + 40))){
		AngleToggle = false;
	}
	else if((RealAngle <= (SensorValue[in1] - 40))){
		motor[port5] = 66;
	}
	else if((RealAngle >= (SensorValue[in1] + 40))){
		motor[port5] = -66;
	}
}

void AngleLift(){
	if(AngleToggle == true){
		if((SensorValue[in1] >= LiftAngle - 40) && (SensorValue[in1] <= LiftAngle + 40)) {
			motor[Lift1] = 0;
			motor[Lift2] = 0;
			AngleToggle = false;
		}
		if(SensorValue[in1] <= LiftAngle - 40){
			motor[Lift1] = 127;
			motor[Lift2] = 127;
		}
		if(SensorValue[in1] >= LiftAngle + 40){
			motor[Lift1] = -127;
			motor[Lift2] = -127;
		}
	}
}

void Base(){
	motor[FrontLeft] = LeftJoyM + vexRT[Btn5U]*-75 + vexRT[Btn6U]*75;
	motor[FrontRight] = RightJoyM + vexRT[Btn5U]*75 + vexRT[Btn6U]*-75;
	motor[BackLeft] = LeftJoyM + vexRT[Btn5U]*75 + vexRT[Btn6U]*-75;
	motor[BackRight]= RightJoyM + vexRT[Btn5U]*-75 + vexRT[Btn6U]*75;
}

void Lift(){
	if(vexRT[Btn8UXmtr2] == 1){
		LiftAngle = 3270;
		AngleToggle = true;
	}
	else if(vexRT[Btn8DXmtr2] == 1){
		LiftAngle = 1820;
		AngleToggle = true;
	}

	AngleCorrect();
	AngleLift();
	if(AngleToggle == false){
		motor[Lift1]= RightJoyS;
		motor[Lift2]= RightJoyS;
	}
}

void Manipulator(){
	motor[Manipulator1] = LeftJoyS;
	motor[Manipulator2] = LeftJoyS;
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
