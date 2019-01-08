void StopAndWait(int n)
{
	motor[motorB] = 0;
	motor[motorC] = 0;
	wait1Msec(n);
}

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

void RightAlign(int turn)
{
	StopAndWait(500);

	ZeroEnc();
	Motors(-20, -20);
	while(nMotorEncoder[motorB] > -220) wait1Msec(1);
	StopAndWait(500);

	ZeroEnc();
	Motors(70, -20);
	while(nMotorEncoder[motorB] < turn) wait1Msec(1);
	StopAndWait(500);

	Motors(20, 20);
	wait1Msec(900);
	StopAndWait(0);
}

void LeftAlign(int turn)
{
	StopAndWait(500);

	ZeroEnc();
	Motors(-20, -20);
	while(nMotorEncoder[motorB] > -220) wait1Msec(1);
	StopAndWait(500);

	ZeroEnc();
	Motors(-20, 70);
	while(nMotorEncoder[motorC] < turn) wait1Msec(1);
	StopAndWait(500);

	Motors(20, 20);
	wait1Msec(900);
	StopAndWait(0);
}

void ReverseDriving(int enc)
{
	ZeroEnc();
	Motors(-20, -20);
	while((nMotorEncoder[motorB] > enc) || (nMotorEncoder[motorC] > enc)) wait1Msec(1);
	Motors(0, 0);
}

void RightSpecialTurn(int u_input, int k_turn)
{
	PlaySound(soundDownwardTones);
	StopAndWait(700);

	ZeroEnc();
	Motors(20, -20);
	int to_turn = u_input*k_turn;
	if (abs(to_turn) > 480)
		to_turn = (to_turn * 480) / abs(to_turn);
	while(abs(nMotorEncoder[motorB]) < abs(to_turn)) wait1Msec(1);
	StopAndWait(700);

	ZeroEnc();
	Motors(20, 20);
	while(nMotorEncoder[motorB] < 300) wait1Msec(1);
	StopAndWait(700);

	ZeroEnc();
	Motors(-20, 20);
	while(abs(nMotorEncoder[motorC]) < abs(to_turn)) wait1Msec(1);
	StopAndWait(700);
}

void LeftSpecialTurn(int u_input, int k_turn)
{
	PlaySound(soundUpwardTones);
	StopAndWait(700);

	ZeroEnc();
	Motors(-20, 20);
	int to_turn = u_input*k_turn;
	if (abs(to_turn) > 480)
		to_turn = (to_turn * 480) / abs(to_turn);
	while(abs(nMotorEncoder[motorC]) < abs(to_turn)) wait1Msec(1);
	StopAndWait(700);

	ZeroEnc();
	Motors(20, 20);
	while(nMotorEncoder[motorB] < 300) wait1Msec(1);
	StopAndWait(700);

	ZeroEnc();
	Motors(20, -20);
	while(abs(nMotorEncoder[motorB]) < abs(to_turn)) wait1Msec(1);
	StopAndWait(700);
}
