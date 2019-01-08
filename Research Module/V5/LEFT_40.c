#pragma config(Sensor, S1,     Touch1,         sensorTouch)
#pragma config(Sensor, S2,     Sonar1,         sensorSONAR)
#pragma config(Sensor, S3,     HTAC,           sensorI2CCustom)
#pragma config(Sensor, S4,     Touch2,         sensorTouch)
#pragma config(Motor,  motorA,          bur1,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          bur2,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          Motor1,        tmotorNXT, PIDControl, encoder)

void input()
{
	motor[bur1] = 100;
	motor[bur2] = 100;
	while((SensorValue[Touch1] == 0) && (SensorValue[Touch2] == 0)) wait1Msec(1);
	ClearTimer(T1);
	while(time1[T1] < 2000){
		if(SensorValue[Touch1] == 0){
			motor[bur2] = 20;
			motor[bur1] = 100;
		}
		if(SensorValue[Touch2] == 0){
			motor[bur2] = 100;
			motor[bur1] = 20;
		}
		wait1Msec(1);
	}
	motor[Bur1] = 100;
	motor[Bur2] = 100;
	wait1Msec(500);
	motor[Bur1] = 0;
	motor[Bur2] = 0;
}

task main()
{
	input();
}
