#pragma config(Sensor, S1,     Touch1,         sensorTouch)
#pragma config(Sensor, S2,     Sonar1,         sensorSONAR)
#pragma config(Sensor, S3,     HTCS2,          sensorI2CCustom)
#pragma config(Sensor, S4,     Touch2,         sensorTouch)
#pragma config(Motor,  motorA,          Bur1,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          Bur2,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          Motor1,        tmotorNXT, PIDControl, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//Controls Motor1 on LEFT NXT
#include "drivers/hitechnic-colour-v2.h"

int position;
int inputMessage = 0;
int motorSpeed = 0;
int color = 17;
int type = 0;
int steps = 0;
int out = 100;
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

void waitForMessage(int pos)
{
	inputMessage = 0;
	while(inputMessage != pos)
	{
		if(cCmdMessageGetSize(mailbox1) != 0)
		{
			inputMessage = messageParm[0];
			ClearMessage();
		}
		wait1Msec(30);
	}
}

void CheckAll()
{
	color = HTCS2readColor(HTCS2);
	if ((SensorValue[Sonar1] > 60) || (color == 2) || (color == 8) || (color == 9) || (color == 10))
	{
		if(SensorValue[Sonar1] > 60)
		{
			PlaySound(soundLowBuzz);
			PlaySound(soundException);
			PlaySound(soundUpwardTones);
			out = -1;
			type = 6;
		}
		else{
			type = 5;
			if(out > 4){
				sound = soundUpwardTones;
				StartTask(soundPlay);
				out = 3;
			}
		}
	}
}

task screen()
{
	while(true)
	{
		nxtDisplayBigTextLine(2, "Pos:  %d", position);
		nxtDisplayBigTextLine(4, "Speed:%d", motorSpeed);
		nxtDisplayBigTextLine(6, "Color:%d", color);
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
	ClearMessage();
	position = -1;
	StartTask(screen);
	while(messageParm[0] != 239) wait1Msec(1);
	position = 0;
	PlaySound(soundBeepBeep);
	input();
	wait1Msec(12000);
	StartTask(sendPosition);
	StartTask(motorMove);
	while(true)//idem k metke
	{
		position = 4;
		wait1Msec(1000);
		if(out <= 0) break;
		output(3000);
		position = 1;
		motorSpeed = 15;
		wait1Msec(1000);
		motorSpeed = 10;
		input();
		motorSpeed = 0;
		position = 2;
		waitForMessage(3);
		position = 3;
		motorSpeed = 30;
		wait1Msec(1000);
		motorSpeed = 10;
		waitForMessage(4);
		motorSpeed = 0;
		CheckAll();
		out = out - 1;
		steps = steps + 1;
	}
	StopTask(soundPlay);

	if(type == 5){
		position = 5;
		sound = soundDownwardTones;
		StartTask(soundPlay);
		wait1Msec(7000);
		StopTask(soundPlay);
		//2 more steps
	}
	if(type == 6){
		position = 6;
		sound = soundBeepBeep;
		StartTask(soundPlay);
		wait1Msec(4000);
		StopTask(soundPlay);
		wait1Msec(200000);
	}
	ClearMessage();
	PlaySound(soundBeepBeep);
	wait1Msec(23000);
	PlaySound(soundBeepBeep);
	//idem k verhu----------------------------------------------------------------------------------
	/*
	out = 100;
	type = 0;
	while(true)
	{
		position = 4;
		wait1Msec(1000);
		if(out > 0)
			position = 1;
		else
			break;
		output(8000);
		motorSpeed = speedStandart;
		wait1Msec(1000);
		input();
		position = 2;
		waitForMessage(3);
		position = 3;
		motorSpeed = speedStandart;
		wait1Msec(1000);
		waitForMessage(4);
		CheckAll();
		out = out - 1;
		steps = steps + 1;
	}
	StopTask(soundPlay);

	if(type == 5){
		position = 5;
		sound = soundDownwardTones;
		StartTask(soundPlay);
		wait1Msec(7000);
		StopTask(soundPlay);
		//2 more steps
	}
	if(type == 6){
		position = 6;
		sound = soundBeepBeep;
		StartTask(soundPlay);
		wait1Msec(4000);
		StopTask(soundPlay);
	}
	*/
}
