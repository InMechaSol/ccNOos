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

#include "../../ccNOos/executionSystem/execution_system.h"
#include "../../ccNOos/consoleMenu/console_menu.h"

//void SysTickISRCallback(void); // not using on QTCreatorC
#define LIGHT_OFF (1u)      // 1-PSoC4, 0-most others
#define uSEC_PER_CLOCK (1000000/CLOCKS_PER_SEC)
#define MAXLINELENGTH (80)


// 0) (Optional) Platform Config and Log Files/Devices
// 1) Platform Setup Function
void platformSetup()
{
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
    usleep(100000);
    //</platformLoopDelay>
}

#ifdef __USINGCONSOLEMENU
// 4) Basic ability for user console input
void GetMenuChars(char* inStringPtr)
{
    int ch = 0;
    int retVal = 1;
    while(ch < MAXLINELENGTH)
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
}
// 5) Basic ability for user console output
void WriteMenuLine(char* outStringPtr)
{
    int retVal = printf(outStringPtr);
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

