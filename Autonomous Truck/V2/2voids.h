void Motors(int mmB, int mmC)
{
	motor[motorB] = mmB;
	motor[motorC] = mmC;
}

void ZeroEnc()
{
	nMotorEncoder[motorB] = 0;
	nMotorEncoder[motorC] = 0;
}

void setModule()
{
	wait1Msec(1000);
	ZeroEnc();
	Motors(10, 10);
	while((SensorValue[Touch1] == 0) || (SensorValue[Touch2] == 0)) wait1Msec(1);
	Motors(0, 0);
}

void setPlatform()
{
	ZeroEnc();
	Motors(-20, -20);
	while((nMotorEncoder[motorB] > -500) || (nMotorEncoder[motorB] > -500)) wait1Msec(1);
	Motors(0, 0);
}

bool isAllTouched()
{
	if((SensorValue[Touch1] == 1) && (SensorValue[Touch2] == 1))
		return true;
	else
		return false;
}

bool isAnyTouched()
{
	if((SensorValue[Touch1] == 1) || (SensorValue[Touch2] == 1))
		return true;
	else
		return false;
}
