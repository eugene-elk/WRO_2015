#pragma config(Sensor, S1,     Touch1,         sensorTouch)
#pragma config(Sensor, S2,     Touch2,         sensorTouch)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

ubyte data[100];
int u = 0;
int position = 0;
int turn = 180;
int kMain = 0.5;

void MotorsBC(int mmB, int mmC)
{
	motor[motorB] = mmB;
	motor[motorC] = mmC;
}

void ZeroEncBC()
{
	nMotorEncoder[motorB] = 0;
	nMotorEncoder[motorC] = 0;
}

task BT()
{
	while(true)
  {
    while (cCmdMessageGetSize(mailbox1) < 1) wait1Msec(1);
    cCmdMessageRead(data, 2, mailbox1);
    u = data[0] - 100;
    wait1Msec(30);
  }
}

task PregCamera()
{
	int mA = 0;
	nMotorEncoder[motorA] = 0;
	while(true)
	{
		mA = ((data[1] - 100) - nMotorEncoder[motorA]) * 0.1;
		motor[motorA] = mA;
		wait1Msec(10);
	}
}

void StopAndWait(int n)
{
	motor[motorB] = 0;
	motor[motorC] = 0;
	wait1Msec(n);
}

task Screen()
{
	while(true)
	{
		nxtDisplayCenteredBigTextLine(3, "%d", position);
		wait1Msec(30);
	}
}

task SmartAlign()
{
	int kTurn = 10;
	while(true)
	{
		if (abs(u) <= 4)
		{
			PlaySound(soundBlip);
			MotorsBC(20 + (kMain*u), 20 - (kMain*u));
		}
		else
		{
			if (u > 0)
			{
				PlaySound(soundUpwardTones);

				nMotorEncoder[motorB] = 0;
				MotorsBC(70, -70);
				while(abs(nMotorEncoder[motorB]) < abs(u*kTurn)) wait1Msec(1);
				StopAndWait(100);

				PlaySound(soundUpwardTones);
				nMotorEncoder[motorB] = 0;
				MotorsBC(40, 40);
				while(nMotorEncoder[motorB] < 500) wait1Msec(1);
				StopAndWait(100);

				PlaySound(soundUpwardTones);
				nMotorEncoder[motorC] = 0;
				MotorsBC(-70, 70);
				while(abs(nMotorEncoder[motorC]) < abs(u*kTurn)) wait1Msec(1);
				StopAndWait(100);
			}
			else
			{
				PlaySound(soundDownwardTones);

				nMotorEncoder[motorC] = 0;
				MotorsBC(-70, 70);
				while(abs(nMotorEncoder[motorC]) < abs(u*kTurn)) wait1Msec(1);
				StopAndWait(100);

				PlaySound(soundDownwardTones);
				nMotorEncoder[motorB] = 0;
				MotorsBC(40, 40);
				while(nMotorEncoder[motorB] < 500) wait1Msec(1);
				StopAndWait(100);

				PlaySound(soundDownwardTones);
				nMotorEncoder[motorB] = 0;
				MotorsBC(70, -70);
				while(abs(nMotorEncoder[motorB]) < abs(u*kTurn)) wait1Msec(1);
				StopAndWait(100);
			}
		}
		wait1Msec(30);
	}
}

task Align()
{
	turn = 120;
	while(true)
	{
		if((SensorValue[Touch1] == 0)&&(SensorValue[Touch2] == 0))
		{
			MotorsBC(50, 50);
		}
		else
		{
			if(SensorValue[Touch1] == 1)
			{
				StopAndWait(1000);

				ZeroEncBC();
				MotorsBC(-30, -30);
				while(nMotorEncoder[motorB] > -220) wait1Msec(1);
				StopAndWait(0);

				ZeroEncBC();
				MotorsBC(70, -20);
				while(nMotorEncoder[motorB] < turn) wait1Msec(1);
				StopAndWait(0);

				MotorsBC(40, 40);
				wait1Msec(700);
				StopAndWait(0);
				turn -= 20;
				if(turn < 40) turn = 40;
			}
			else
			{
				if(SensorValue[Touch2] == 1)
				{
					StopAndWait(1000);

					ZeroEncBC();
					MotorsBC(-30, -30);
					while(nMotorEncoder[motorB] > -220) wait1Msec(1);
					StopAndWait(0);

					ZeroEncBC();
					MotorsBC(-20, 70);
					while(nMotorEncoder[motorC] < turn) wait1Msec(1);
					StopAndWait(0);

					MotorsBC(40, 40);
					wait1Msec(700);
					StopAndWait(0);
					turn -= 20;
					if(turn < 40) turn = 40;
				}
			}
		}
		wait1Msec(1);
	}
}

task main()
{
	StartTask(Screen);
	StartTask(BT);
	StartTask(PregCamera);
	while((SensorValue[Touch1] == 0) && (SensorValue[Touch2] == 0))
	{
		motor[motorB] = 15 + (kMain*u);
		motor[motorC] = 15 - (kMain*u);
		wait1Msec(30);
	}
	PlaySound(soundBeepBeep);
	StopAndWait(1000);
	StartTask(Align);
	while((SensorValue[Touch1] == 0) || (SensorValue[Touch2] == 0)) wait1Msec(1);
	StopTask(Align);
	StopAndWait(0);

	position = 1;
	PlaySound(soundBeepBeep);
	wait1Msec(500);
	ZeroEncBC();
	motor[motorB] = -30;
	motor[motorC] = -30;
	while((nMotorEncoder[motorB] > -1600) || (nMotorEncoder[motorC] > -1600)) wait1Msec(1);//otezd daleko
	StopAndWait(2000);

	StartTask(SmartAlign);
	while((SensorValue[Touch1] == 0) && (SensorValue[Touch2] == 0)) wait1Msec(1);//podezd
	StopTask(SmartAlign);

	PlaySound(soundBeepBeep);
	StopAndWait(1000);
	StartTask(Align);//vyravnivanie
	while((SensorValue[Touch1] == 0) || (SensorValue[Touch2] == 0)) wait1Msec(1);
	StopTask(Align);
	StopAndWait(0);

	PlaySound(soundBeepBeep);
	wait1Msec(500);
	ZeroEncBC();
	motor[motorB] = -30;
	motor[motorC] = -30;
	while((nMotorEncoder[motorB] > -450) || (nMotorEncoder[motorC] > -450)) wait1Msec(1);//otezd
	StopAndWait(2000);

	position = 2;//spusk robota
	PlaySound(soundBeepBeep);
	while((SensorValue[Touch1] == 0) || (SensorValue[Touch2] == 0)) wait1Msec(1);//spusk robota
	position = 3; //vburivanie
  PlaySound(soundBeepBeep);
  wait1Msec(2000);
	ZeroEncBC();
	motor[motorB] = 80;
	motor[motorC] = 80;
	while((nMotorEncoder[motorB] < 180) || (nMotorEncoder[motorC] < 180)) wait1Msec(1);
	PlaySound(soundBeepBeep);
	motor[motorB] = 60;
	motor[motorC] = 60;
	wait1Msec(8000);//vburivanie po taimeru
	motor[motorB] = 0;
	motor[motorC] = 0;

	position = 4;//opening
	PlaySound(soundBeepBeep);

	wait1Msec(1000);

	position = 5;//otezd posle vburivania
	PlaySound(soundBeepBeep);

	wait1Msec(500);

	ZeroEncBC();
	motor[motorB] = -30;
	motor[motorC] = -30;
	while((nMotorEncoder[motorB] > -360) || (nMotorEncoder[motorC] > -360)) wait1Msec(1);
	motor[motorB] = 0;
	motor[motorC] = 0;

	position = 6;//ubiranie shvata
  PlaySound(soundBeepBeep);
}
