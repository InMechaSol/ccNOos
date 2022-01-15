#if PLATFORM_NAME!=Arduino
    error PLATFORM_NAME must be Arduino
#endif


#ifdef REDEFINE_NULLPTR
    #error Must not compile with -DREDEFINE_NULLPTR on Aduino ATMega (maybe others, need to test)
#endif // !REDEFINE_NULLPTR
#ifndef __NOEXCEPTIONS
    #error Must compile with -D__NOEXCEPTIONS on Aduino ATMega (maybe others, need to test)
#endif // !__NOEXCEPTIONS


#include <Arduino.h>
#include <Adafruit_MCP23017.h>
#include "executionSystem/execution_system.h"    
#include "consoleMenu/console_menu.h" 
#define LIGHT_OFF (0u)      // 1-PSoC4, 0-most others
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
    Serial.begin(115200);
    Wire.begin();

    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
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
    ;
    //</platformLoopDelay>
}
#ifdef __USINGCONSOLEMENU
// 4) Basic ability for user console input
void GetMenuChars(char* inStringPtr)
{
    int idx = 0;
    if (Serial.available() > 0)
    {
        do {
            inStringPtr[idx++] = Serial.read();
            //WriteMenuLine((char*)"Debugging: ");
            //WriteMenuLine(inStringPtr);
            //WriteMenuLine((char*)"\n");
        } while (Serial.available() > 0 && idx < MAXLINELENGTH);
        delay(1);
        while (Serial.available() > 0 && idx < MAXLINELENGTH) {
            inStringPtr[idx++] = Serial.read();
            //WriteMenuLine((char*)"Debugging: ");
            //WriteMenuLine(inStringPtr);
            //WriteMenuLine((char*)"\n");
        } 
        delay(2);
        while (Serial.available() > 0 && idx < MAXLINELENGTH) {
            inStringPtr[idx++] = Serial.read();
            //WriteMenuLine((char*)"Debugging: ");
            //WriteMenuLine(inStringPtr);
            //WriteMenuLine((char*)"\n");
        }
    }
    inStringPtr[idx] = 0x00;
    //if (idx > 0)
    //{
    //    WriteMenuLine((char*)"Debugging: ");
    //    WriteMenuLine(inStringPtr);
    //    WriteMenuLine((char*)"\n");
    //}
}
// 5) Basic ability for user console output
void WriteMenuLine(char* outStringPtr)
{
    Serial.write(outStringPtr);
}
// 6) (Optional) Logging Output
void WriteLogLine(char* outStringPtr)
{
    ;
}
// 7) (Optional) Config Input
void ReadConfigLine(char* inStringPtr)
{
    ;

}
// 8) Platform API Functions (From Template?)
PlatformAPIFuncsTemplate(size + 1);
#endif
// 9) Global Execution System Instance
//executionSystemClass PLATFORM_EXESYS_NAME(PLATFORM_NAME)(uSEC_PER_CLOCK);
// 10) ExeSys API Functions (From Template?)
ExeSysAPIFuncsTemplate