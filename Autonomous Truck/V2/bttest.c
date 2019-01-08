ubyte data[100];

task BT()
{
	while(true)
  {
    while (cCmdMessageGetSize(mailbox1) < 1) wait1Msec(1);
    cCmdMessageRead(data, 2, mailbox1);
    nxtDisplayTextLine(1, "%d", data[0]);
    nxtDisplayTextLine(2, "%d", data[1]);
    wait1Msec(30);
  }
}

task main()
{
	StartTask(BT);
}
