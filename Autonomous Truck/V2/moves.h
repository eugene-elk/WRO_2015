void StopAndWait(int n)
{
	motor[motorB] = 0;
	motor[motorC] = 0;
	wait1Msec(n);
}

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
