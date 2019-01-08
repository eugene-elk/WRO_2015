#pragma config(Sensor, S1,     Touch1,         sensorTouch)
#pragma config(Sensor, S2,     Sonar1,         sensorSONAR)
#pragma config(Sensor, S4,     Touch2,         sensorTouch)
#pragma config(Motor,  motorA,          Bur1,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          Bur2,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          Motor1,        tmotorNXT, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//Motor1 is under control of RIGHT NXT
#include "drivers/hitechnic-accelerometer.h"

int position;
int inputMessage = 0;
int motorSpeed = 0;
int steps = 0;
TSounds sound;

task soundPlay()
{
	while(true)
	{
		PlaySound(sound);
		wait1Msec(700);
	}
}

void input()
{
	motor[Bur1] = 100;
	motor[Bur2] = 100;
	while((SensorValue[Touch1] == 0) && (SensorValue[Touch2] == 0)) wait1Msec(1);
	ClearTimer(T1);
	while(time1[T1] < 4000){
		if(SensorValue[Touch1] == 0){
			motor[Bur2] = 20;
			motor[Bur1] = 100;
		}
		if(SensorValue[Touch2] == 0){
			motor[Bur2] = 100;
			motor[Bur1] = 20;
		}
		if((SensorValue[Touch1] == 1) && (SensorValue[Touch2] == 1)) break;
		wait1Msec(1);
	}
	motor[Bur1] = 100;
	motor[Bur2] = 100;
	wait1Msec(500);
	motor[Bur1] = 0;
	motor[Bur2] = 0;
}


void output(int t)
{
	 	motor[Bur1] = -100;
		motor[Bur2] = -100;
		wait1Msec(t);
		motor[Bur1] = 0;
		motor[Bur2] = 0;
		PlaySound(soundBeepBeep);
}

task sendPosition()
{
		while(true)
		{
			sendMessageWithParm(position, 0, 0);
			wait1Msec(30);
		}
}

void waitForMessage(int pos1, int pos2, int pos3)
{
	inputMessage = 0;
	while((inputMessage != pos1) && (inputMessage != pos2) && (inputMessage != pos3) )
	{
		if(cCmdMessageGetSize(mailbox1) != 0)
		{
			inputMessage = messageParm[0];
			ClearMessage();
		}
		wait1Msec(30);
	}
}

task screen()
{
	while(true)
	{
		nxtDisplayBigTextLine(2, "Pos: %d", position);
		nxtDisplayBigTextLine(4, "Speed: %d", motorSpeed);
		wait1Msec(1);
	}
}

task motorMove()
{
	while(true)
	{
		motor[Motor1] = motorSpeed;
		wait1Msec(30);
	}
}

task main()
{
	position = -1;
	StartTask(screen);
	PlaySound(soundBeepBeep);
	position = 0;
	input();
	wait1Msec(12000);
	StartTask(sendPosition);
	StartTask(motorMove);
	//idem k metke --------------------------------------------------------------------------------------
	while(true)
	{
		position = 4;
		wait1Msec(1000);
		waitForMessage(1, 5, 6);
		if((inputMessage == 5)||(inputMessage == 6)) break;
		position = 1;
		motorSpeed = 15;
		wait1Msec(1000);
		waitForMessage(2, 2, 2);
		motorSpeed = 0;
		position = 2;
		output(3000);
		position = 3;
		nMotorEncoder[Motor1] = 0;
		motorSpeed = 15;
		wait1Msec(1000);
		input();
		motorSpeed = 0;
		steps = steps + 1;
  }
	position = 6;
	sound = soundBeepBeep;
	StartTask(soundPlay);
	wait1Msec(4000);
	StopTask(soundPlay);
	ClearMessage();
	//idem nazad ----------------------------------------------------------------------------------
	while(true)
	{
	  position = 2;
		output(3000);
		position = 3;
		nMotorEncoder[Motor1] = 0;
		motorSpeed = -10;
		wait1Msec(1000);
		input();
		motorSpeed = 0;

		position = 4;
		wait1Msec(1000);
		waitForMessage(1, 5, 6);
		if((inputMessage == 5)||(inputMessage == 6)) break;
		position = 1;
		motorSpeed = -5;
		wait1Msec(1000);
		waitForMessage(2, 2, 2);
		motorSpeed = 0;

		steps = steps - 1;
  }
	position = 6;
	sound = soundBeepBeep;
	StartTask(soundPlay);
	wait1Msec(4000);
	StopTask(soundPlay);
}
