#pragma config(Sensor, S3,     Touch1,         sensorTouch)
#pragma config(Sensor, S4,     Touch2,         sensorTouch)
#pragma config(Motor,  motorA,          mA,            tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          mB,            tmotorNXT, PIDControl, reversed, encoder)
#pragma config(Motor,  motorC,          mC,            tmotorNXT, PIDControl, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int position = 0;

#include "2tasks.h"

task main()
{
	StartTask(screen);
	StartTask(holder);

	//PD-reg
	position = 1;
	while(isAnyTouched() == false) wait1Msec(1);

	//Aligning
	position = 2;
	while(isAllTouched() == false) wait1Msec(1);

	//Reverse driving
	position = 3;
	wait1Msec(1000);

	//Special-reg
	position = 4;
	while(isAnyTouched() == false) wait1Msec(1);

	//Aligning. Again.
	position = 5;
	while(isAllTouched() == false) wait1Msec(1);

	//Small reverse driving
	position = 6;
	wait1Msec(2000);

	//Setting Research Module
	position = 7;
	setModule();

	//Drilling
	PlaySound(soundDownwardTones);
	position = 8;
	wait1Msec(12000);
	hold = OPENED;
	wait1Msec(1000);

	//Reverse driving
	PlaySound(soundDownwardTones);
	position = 9;
	wait1Msec(5000);

	//Set platform back
	hold = CLOSED;
	position = 10;
	setPlatform();
}