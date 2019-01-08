#include "1voids.h"

ubyte data[100];
int u_input = 0;

task bt()
{
	while(true)
	{
		while (cCmdMessageGetSize(mailbox1) < 1) wait1Msec(1);
    cCmdMessageRead(data, 3, mailbox1);
    u_input = data[0] - 100;
    wait1Msec(30);
	}
}

task camera()
{
	int motA = 0;
	nMotorEncoder[cam] = 0;
	while(true)
	{
		motA = ((data[1] - 100) - nMotorEncoder[motorA]) * 0.1;
		motor[cam] = motA;
		wait1Msec(10);
	}
}

task screen()
{
	while(true)
	{
		nxtDisplayCenteredBigTextLine(2, "%d", position);
		nxtDisplayCenteredBigTextLine(4, "%d", data[1] - 100);
		wait1Msec(30);
	}
}

task align()
{
	int turn = 80;
	while(true)
	{
		if((SensorValue[Touch1] == 0)&&(SensorValue[Touch2] == 0))
		{
			Motors(40, 40);
		}
		else
		{
			if(SensorValue[Touch1] == 1)
			{
				RightAlign(turn);
				PlaySound(soundBeepBeep);
				turn -= 20;
				if (turn <= 20) turn = 20;
			}
			else if(SensorValue[Touch2] == 1)
			{
				LeftAlign(turn);
				PlaySound(soundBeepBeep);
				turn -= 20;
				if (turn <= 20) turn = 20;
			}
		}
		wait1Msec(1);
	}
}

task SpecialReg()
{
	int k_turn = 12;
	int u, u_old;
	float k1 = 0.7, k2 = 0.8;
	int speed = 20;
	while(true)
	{
		if (abs(u_input) <= 7)
		{
			u = k1 * u_input + k2 * (u_input - u_old);
			Motors(speed + u, speed - u);
		}
		else
		{
			if (u_input > 0)
			{
				RightSpecialTurn(u_input, k_turn);
			}
			else
			{
				LeftSpecialTurn(u_input, k_turn);
			}
		}
		u_old = u_input;
		wait1Msec(1);
	}
}

task alignSmall()
{
	int turn = 60;
	while(true)
	{
		if((SensorValue[Touch1] == 0)&&(SensorValue[Touch2] == 0))
		{
			Motors(40, 40);
		}
		else
		{
			if(SensorValue[Touch1] == 1)
			{
				RightAlign(turn);
				PlaySound(soundBeepBeep);
				turn -= 20;
				if (turn <= 20) turn = 20;
			}
			else if(SensorValue[Touch2] == 1)
			{
				LeftAlign(turn);
				PlaySound(soundBeepBeep);
				turn -= 20;
				if (turn <= 20) turn = 20;
			}
		}
		wait1Msec(1);
	}
}
