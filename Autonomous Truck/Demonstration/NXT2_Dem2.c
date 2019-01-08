#pragma config(Motor,  motorA,          mA,            tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          mB,            tmotorNXT, PIDControl, reversed, encoder)
#pragma config(Motor,  motorC,          mC,            tmotorNXT, PIDControl, reversed, encoder)

#define CLOSED (20)
#define OPENED (-100)

int hold = CLOSED;

task holder()
{
	nMotorEncoder[mA] = 0;
	while(true)
	{
		motor[mA] = 6*(hold - nMotorEncoder[mA]);
		wait1Msec(1);
	}
}


task main()
{
	StartTask(holder);
	wait1Msec(3000);
	for(int x = 1; x <= 1; x++)
	{
		nMotorEncoder[motorC] = 0;
		nMotorEncoder[motorB] = 0;
		motor[mB] = 10;
		motor[mC] = 10;
		while(nMotorEncoder[motorB] < 500) wait1Msec(1);
		motor[mB] = 0;
		motor[mC] = 0;
		wait1Msec(500);
		hold = OPENED;
		wait1Msec(1000);
		motor[mB] = -10;
		motor[mC] = -10;
		while(nMotorEncoder[motorB] > 0){
			if (nMotorEncoder[motorB] < 150) hold = CLOSED;
			wait1Msec(1);
		}
		motor[mB] = 0;
		motor[mC] = 0;
		wait1Msec(500);
	}
}
