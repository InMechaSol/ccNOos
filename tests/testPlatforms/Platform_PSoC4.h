#if PLATFORM_NAME!=PSoC4
    error PLATFORM_NAME must be PSoC4 
#endif


#ifdef REDEFINE_NULLPTR
    #error Must not compile with -DREDEFINE_NULLPTR on PSoC4
#endif // !REDEFINE_NULLPTR
#ifdef __NOEXCEPTIONS
    #error Must not compile with -D__NOEXCEPTIONS on PSoC4
#endif // !__NOEXCEPTIONS


#include <project.h>      // for PSoC
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>

#include "../../executionSystem/execution_system.h"    
#include "../../consoleMenu/console_menu.h" 

void SysTickISRCallback();
#define LIGHT_OFF (1u)      // 1-PSoC4, 0-most others
#define uSEC_PER_CLOCK (1000u)
#define MAXLINELENGTH (80)


// 0) (Optional) Platform Config and Log Files/Devices
// 1) Platform Setup Function
void platformSetup()
{      
    //<platformSetup>
#ifdef __USINGCONSOLEMENU
#ifdef __USINGFLOATPRINTF
    asm(".global _printf_float");
#endif
#endif
    /* Enable global interrupts. */
    CyGlobalIntEnable;
    //</platformSetup>
}
// 2) Platform Start Function
void platformStart()
{
    //<platformStart>
    /* Configure the SysTick timer to generate interrupt every 1 ms
    * and start its operation. Call the function before assigning the
    * callbacks as it calls CySysTickInit() during first run that 
    * re-initializes the callback addresses to the NULL pointers.
    */
    CySysTickStart();

    /* Find unused callback slot and assign the callback. */
    for (int i = 0u; i < (int)CY_SYS_SYST_NUM_OF_CALLBACKS; ++i)
    {
        if (CySysTickGetCallback(i) == NULL)
        {
            /* Set callback */
            CySysTickSetCallback(i, SysTickISRCallback);
            break;
        }
    }
    UART_Start();
    //</platformStart>
}
// 3) Platform Loop Delay Function
void platformLoopDelay()
{    
    //<platformLoopDelay>
    //CyDelay(1);   // on PSoc, only if desired
    ;               // let it run full throttle, its a machine after all...
    //</platformLoopDelay>
}

#ifdef __USINGCONSOLEMENU
// 4) Basic ability for user console input
void GetMenuChars(char* inStringPtr)
{
    static UI_32 tTHEN = 0u;
    
    if(tTHEN == 0)
        tTHEN = getuSecTicks();
        
    if((getuSecTicks()-tTHEN)>1000000)
    {
        inStringPtr[0] = ';';
        tTHEN = getuSecTicks();
        inStringPtr[1] = 0x00;
    }
    else
        inStringPtr[0] = 0x00;
}
// 5) Basic ability for user console output
void WriteMenuLine(char* outStringPtr)
{
    UART_PutString(outStringPtr);
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
PlatformAPIFuncsTemplate(size + 1);
#endif
// 9) Global Execution System Instance
//PLATFORM_EXESYS_DECLARE(PLATFORM_NAME);
// 10) ExeSys API Functions (From Template?)
ExeSysAPIFuncsTemplate
