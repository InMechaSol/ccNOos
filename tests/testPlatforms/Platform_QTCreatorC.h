#if PLATFORM_NAME!=QTCreatorC
    error PLATFORM_NAME must be QTCreatorC
#endif


#ifdef REDEFINE_NULLPTR
#error Must not compile with -DREDEFINE_NULLPTR on QTCreatorC
#endif // !REDEFINE_NULLPTR
#ifdef __NOEXCEPTIONS
#error Must not compile with -D__NOEXCEPTIONS on QTCreatorC
#endif // !__NOEXCEPTIONS


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>
#include <pthread.h>

#include "execution_system.h"
#include "console_menu.h"

//void SysTickISRCallback(void); // not using on QTCreatorC
#define LIGHT_OFF (1u)      // 1-PSoC4, 0-most others
#define uSEC_PER_CLOCK (1000000/CLOCKS_PER_SEC)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// QTCreator_C is a special platfrom, it's a ccNOos test platform but, can link-in os features at the main file
// - it can also run on either linux or windows, this is a unique test platform in that regard
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "serial_comms.h"
#include "adafruit_ft232h.h"

struct portParametersStruct GPSPortParams;
struct portParametersStruct eCompPortParams;

// 0) (Optional) Platform Config and Log Files/Devices
// 1) Platform Setup Function
void platformSetup()
{

#ifdef _WIN32
    GPSPortParams = buildportParametersStruct("\\\\.\\COM27",9600);
    eCompPortParams = buildportParametersStruct("\\\\.\\COM26",19200);
#else
    GPSPortParams = buildportParametersStruct("/dev/tty1",9600);
    eCompPortParams = buildportParametersStruct("/dev/tty2",19200);
#endif

    openComPort(&GPSPortParams);
    openComPort(&eCompPortParams);

    //<platformSetup>
    //
    // open config device


    // read config string??
    //
    // open log device
    // wrtie log string??
    //
    //</platformSetup>
}
// 2) Platform Start Function
void platformStart()
{
    //<platformStart>
    //</platformStart>
}
// 3) Platform Loop Delay Function
void platformLoopDelay()
{
    //<platformLoopDelay>
    usleep(1000);
    //</platformLoopDelay>
}

#ifdef __USINGCONSOLEMENU
pthread_t stdInThread;
UI_8 stdInThreadRunning = ui8FALSE;
UI_8 runOnce = ui8TRUE;
UI_8 runONCE = ui8TRUE;
void *readStdIn(void* voidinStringPtr)
{
    char* inStringPtr = (char*)voidinStringPtr;
    int ch = 0;
    int retVal = 1;

    do{
        ch = 0;
        retVal = 1;
        while(ch < charBuffMax)
        {
            retVal = read(STDIN_FILENO, &inStringPtr[ch], 1);
            ch++;
            if  (
                inStringPtr[ch-1] == '\n' ||
                retVal < 1
                )
                break;
        }
        inStringPtr[ch] = 0x00;
        stdInThreadRunning = ui8FALSE;

    }while(1);
    return NULL;
}
// 4) Basic ability for user console input
void GetMenuChars(char* inStringPtr)
{
    if(runOnce == ui8TRUE)
    {
        inStringPtr[0] = ';';
        inStringPtr[1] = '\n';
        inStringPtr[2] = '\0';
        runOnce = ui8FALSE;
    }
    else if(stdInThreadRunning == ui8FALSE && inStringPtr[0] == 0x00)
    {
        if(runONCE)
        {
            if(pthread_create(&stdInThread, NULL, &readStdIn, inStringPtr )==0)
                runONCE = ui8FALSE;
        }
        stdInThreadRunning = ui8TRUE;
    }

}
// 5) Basic ability for user console output
void WriteMenuLine(char* outStringPtr)
{
    if(stdInThreadRunning == ui8FALSE)
    {

        printf(outStringPtr);
    }
}
// 6) (Optional) Logging Output
void WriteLogLine(char* outStringPtr)
{

}
// 7) (Optional) Config Input
void ReadConfigLine(char* inStringPtr)
{


}
// 8) Platform API Functions (From Template?)
PlatformAPIFuncsTemplate(size+1);

#endif
// 9) Global Execution System Instance
//PLATFORM_EXESYS_DECLARE(PLATFORM_NAME);
// 10) ExeSys API Functions (From Template?)
ExeSysAPIFuncsTemplate

