#include "2voids.h"

#define CLOSED (20)
#define OPENED (-100)

int hold = CLOSED;

task holder()
{
	nMotorEncoder[mA] = 0;
	while(true)
	{
		motor[mA] = 4*(hold - nMotorEncoder[mA]);
		wait1Msec(1);
	}
}

task screen()
{
	while(true)
	{
		nxtDisplayCenteredBigTextLine(3, "%d", position);
		wait1Msec(30);
	}
}
