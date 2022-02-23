#if PLATFORM_NAME!=Win32
error PLATFORM_NAME must be Win32
#endif


#ifdef REDEFINE_NULLPTR
#error Must not compile with -DREDEFINE_NULLPTR on WIN32
#endif // !REDEFINE_NULLPTR
#ifdef __NOEXCEPTIONS
#error Must not compile with -D__NOEXCEPTIONS on WIN32
#endif // !__NOEXCEPTIONS


#include <ctime>
#include <thread>
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <fstream>

#include "execution_system.h"    
#include "console_menu.h" 

#define LIGHT_OFF (0u)      // 1-PSoC4, 0-most others
#define uSEC_PER_CLOCK (1000000/CLOCKS_PER_SEC)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Win32 is a special platfrom, it's a ccNOos test platform but, can link-in os features at the main file
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "serial_comms.h"
#include "adafruit_ft232h.h"

struct portParametersStruct GPSPortParams;
struct portParametersStruct eCompPortParams;
nbserial_class GPS_NBSerial(&GPSPortParams);
nbserial_class eComp_NBSerial(&eCompPortParams);

// 0) (Optional) Platform Config and Log Files/Devices
std::ifstream configFile;
std::ofstream LogFile;
// 1) Platform Setup Function
void platformSetup()
{

#ifdef _WIN32
    GPSPortParams = buildportParametersStruct("\\\\.\\COM29", 9600);
    eCompPortParams = buildportParametersStruct("\\\\.\\COM4", 19200);
#else
    GPSPortParams = buildportParametersStruct("/dev/tty1", 9600);
    eCompPortParams = buildportParametersStruct("/dev/tty2", 19200);
#endif

    openComPort(&GPSPortParams);
    openComPort(&eCompPortParams);

    //<platformSetup>
    // 
    // open config device
    configFile.open("conFile.json");
    LogFile.open("logFile.json");
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
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    //</platformLoopDelay>
}
#ifdef __USINGCONSOLEMENU
std::thread stdInThread;
UI_8 stdInThreadRunning = ui8FALSE;
UI_8 runOnce = ui8TRUE;
bool runONCE = true;
void readStdIn(char* inStringPtr)
{
    do {
        if (stdInThreadRunning == ui8TRUE)
        {
            std::cin >> inStringPtr;
            stdInThreadRunning = ui8FALSE;
        }
    } while (true);
}
// 4) Basic ability for user console input
void GetMenuChars(char* inStringPtr)
{
    if (runOnce == ui8TRUE)
    {
        inStringPtr[0] = ';';
        inStringPtr[1] = '\n';
        inStringPtr[2] = '\0';
        runOnce = ui8FALSE;
    }
    else if (stdInThreadRunning == ui8FALSE && inStringPtr[0] == 0x00)
    {
        if (runONCE)
        {
            stdInThread = std::thread(readStdIn, inStringPtr);
            runONCE = false;
        }        
        stdInThreadRunning = ui8TRUE;
    }

}
// 5) Basic ability for user console output
void WriteMenuLine(char* outStringPtr)
{
    if (stdInThreadRunning == ui8FALSE)
    {
        std::cout << outStringPtr;
    }
}
// 6) (Optional) Logging Output
void WriteLogLine(char* outStringPtr)
{
    int logLineLen = -1;
    while (logLineLen < charBuffMax)
        if (outStringPtr[++logLineLen] == 0x00)
            break;
    if (logLineLen > 0)
        LogFile.write(outStringPtr, logLineLen);
}
// 7) (Optional) Config Input
void ReadConfigLine(char* inStringPtr)
{
    int confLineLen = 0;
    while (confLineLen < charBuffMax)
    {
        if (0 < configFile.peek())
            configFile.read(&inStringPtr[confLineLen++], 1);
        else
            break;
    }

}
// 8) Platform API Functions (From Template?)
PlatformAPIFuncsTemplate(size + 1);
#endif
// 9) Global Execution System Instance
//executionSystemClass PLATFORM_EXESYS_NAME(Plat)(uSEC_PER_CLOCK);
// 10) ExeSys API Functions (From Template?)
ExeSysAPIFuncsTemplate