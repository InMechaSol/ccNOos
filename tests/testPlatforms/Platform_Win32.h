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

struct devicedatastruct LCDKeyPadDevDataStruct;
struct devicedatastruct ConsoleMenuDevDataStruct;

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

    LCDKeyPadDevDataStruct = createDeviceStruct();
    ConsoleMenuDevDataStruct = createDeviceStruct();

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

// 8) Platform API Functions (From Template?)
PlatformAPIFuncsTemplate(size + 1);
#endif
// 9) Global Execution System Instance
//executionSystemClass PLATFORM_EXESYS_NAME(Plat)(uSEC_PER_CLOCK);
// 10) ExeSys API Functions (From Template?)
ExeSysAPIFuncsTemplate