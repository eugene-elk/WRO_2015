
task main()
{
	wait1Msec(3000);
	for(int x = 1; x <= 2; x++)
		{
		nMotorEncoder[motorA] = 0;
		motor[motorA] = 10;
		while(nMotorEncoder[motorA] < 90) wait1Msec(1);
		wait1Msec(500);
		motor[motorA] = - 10;
		while(nMotorEncoder[motorA] > 0) wait1Msec(1);
	}
}
