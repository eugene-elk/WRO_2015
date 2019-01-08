class bt
{
    public:
        HANDLE connect(int port)
        {
            HANDLE hCom;
            OVERLAPPED o;
            BOOL fSuccess;
            DWORD dwEvtMask;

            char name[13];
            sprintf(name,"\\\\.\\COM%d",port);

            hCom = CreateFile( name,//мой
                             GENERIC_READ | GENERIC_WRITE,
                             0,    // comm devices must be opened w/exclusive-access
                             NULL, // no security attributes
                             OPEN_EXISTING, // comm devices must use OPEN_EXISTING
                             0,    // overlapped I/O
                             NULL  // hTemplate must be NULL for comm devices
                             );

            if (hCom == INVALID_HANDLE_VALUE)
            {
                // Handle the error.
                printf("CreateFile failed with error %d.\n", GetLastError());
            }

            // Set the event mask.

            fSuccess = SetCommMask(hCom, EV_CTS | EV_DSR);

            if (!fSuccess)
            {
                // Handle the error.
                printf("SetCommMask failed with error %d.\n", GetLastError());
            }

            // Create an event object for use by WaitCommEvent.

            o.hEvent = CreateEvent(
                NULL,   // default security attributes
                TRUE,   // manual-reset event
                FALSE,  // not signaled
                NULL    // no name
                );


            // Initialize the rest of the OVERLAPPED structure to zero.
            o.Internal = 0;
            o.InternalHigh = 0;
            o.Offset = 0;
            o.OffsetHigh = 0;

            assert(o.hEvent);

            COMMTIMEOUTS CommTimeOuts;
            CommTimeOuts.ReadIntervalTimeout= 3;
            CommTimeOuts.ReadTotalTimeoutMultiplier = 1;
            CommTimeOuts.ReadTotalTimeoutConstant = 2;
            CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
            CommTimeOuts.WriteTotalTimeoutConstant = 0;

            SetCommTimeouts(hCom, &CommTimeOuts);

            if (WaitCommEvent(hCom, &dwEvtMask, &o))
            {
                if (dwEvtMask & EV_DSR)
                {
                     // To do.
                }

                if (dwEvtMask & EV_CTS)
                {
                    // To do.
                }
            }
            else
            {
                DWORD dwRet = GetLastError();
                if( ERROR_IO_PENDING == dwRet)
                {
                    printf("I/O is pending...\n");

                    // To do.
                }
                else
                    printf("Wait failed with error %d.\n", GetLastError());
            }
            return hCom;
        }

        void dispatch(HANDLE hCom, int s1, int s2, int s3)
        {
            DWORD bc;
            bc = 0;
            byte str[9];
            memset(str,  0, sizeof(str));

            str[0] = (0x07);	str[1] = (0x00);	str[2] = (0x80);	str[3] = (0x09); // 1 байт - кол-во переменных
            str[4] = (0x00);	str[5] = 3;

            str[6] = s1; str[7] = s2; str[8] = s3;

            bc = 0;
            WriteFile(hCom, str, 9, &bc, NULL);
        }
};

bt bt;
