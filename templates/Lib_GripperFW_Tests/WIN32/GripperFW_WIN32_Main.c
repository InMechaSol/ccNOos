//////////////////////////////////
// Include Application and cNOos
#include "Application_Serialization.h"

////////////////////////////////
// Compile Error if Examples/Tests not defining 
#ifndef Mn
#error Mn must be defined - see examples
#endif
#ifndef MODdeclareCREATEINS
#error MODdeclareCREATEINS must be defined - see examples
#endif
#ifndef MODcallCREATEINS
#error MODcallCREATEINS must be defined - see examples
#endif
#ifdef __cplusplus
#ifndef PLATFORM_APP_CLASS
#error PLATFORM_APP_CLASS macro must be defined in application code
#endif
#else
#ifndef MODdeclareDATA
#error MODdeclareDATA macro must be defined in application code
#endif
#endif

////////////////////////////////
// Compile Error if Examples/Tests not defining
#if PLATFORM_NAME==Win32
#include "Platform_ccOS.hpp"
#else
#error "No Platform Selected for Compile!"
#endif

///////////////////////////////////////////////////////////////////////
// Module Data Structure or Module Data Class 
//   -Declared from Macro Template
//   -Macro Template is Defined in the Application Solution Header
///////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
PLATFORM_APP_CLASS(Mn);
#else
MODdeclareDATA(Mn);
#endif

struct devicedatastruct PacketsInterfactDevDataStruct;
struct devicedatastruct ConsoleMenuDevDataStruct;
struct devicedatastruct SmartMotorsDevDataStruct[NUMMOTORS];


///////////////////////////////////////////////////////////////////////
// Platform and Application Specific IO Device Functions
void linkAPIioDevices(struct ccGripperStruct* gripperStructPtrIn)
{
    int i;
    for (i=0; i<NUMMOTORS; i++)
        gripperStructPtrIn->SmartMotors[i].devptr = &SmartMotorsDevDataStruct[i];
    gripperStructPtrIn->ConsoleMenu.devptr = &ConsoleMenuDevDataStruct;
    gripperStructPtrIn->PacketsAPI.devptr = &PacketsInterfactDevDataStruct;
    ConsoleMenuDevDataStruct.triggerWriteOperation = ui8TRUE;
    gripperStructPtrIn->ConsoleMenu.showHelp = ui8TRUE;    
}

UI_32 getMillis() { 
    return getuSecTicks()/1000; }

std::thread stdInThread;
UI_8 stdInThreadRunning = ui8FALSE;
bool runONCE = true; // to launch std::in thread once
void readStdIn(char* inStringPtr)
{
    do {
        if (stdInThreadRunning == ui8TRUE)
        {
            std::cin >> inStringPtr;
            stdInThreadRunning = ui8FALSE;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (true);
}
// 4) Basic ability for user console input via any io device
void GetMenuChars(struct uiStruct* uiStructPtrin)
{

    // if Consolue Menu
    if (uiStructPtrin->devptr == &ConsoleMenuDevDataStruct)
    {
        if (stdInThreadRunning == ui8FALSE)
        {
            if (runONCE)
            {
                stdInThread = std::thread(readStdIn, &uiStructPtrin->devptr->inbuff.charbuff[0]);
                runONCE = false;
                stdInThreadRunning = ui8TRUE;
            }
            else if (uiStructPtrin->devptr->triggerWriteOperation == ui8FALSE)
            {
                uiStructPtrin->devptr->newDataReadIn = ui8TRUE;
                uiStructPtrin->parseIndex = 0;
                stdInThreadRunning = ui8TRUE;
            }
        }
    }
}
// 5) Basic ability for user console output
void WriteMenuLine(struct uiStruct* uiStructPtrin)
{
    // if Consolue Menu
    if (uiStructPtrin->devptr == &ConsoleMenuDevDataStruct)
    {
        if (uiStructPtrin->clearScreen) {
            std::cout << terminalClearString();
            uiStructPtrin->clearScreen = ui8FALSE;
        }
        std::cout << &uiStructPtrin->devptr->outbuff.charbuff[0];
        if (uiStructPtrin->showPrompt) {
            std::cout << terminalPromptString(uiStructPtrin->currentUserLevel);
            uiStructPtrin->showPrompt = ui8FALSE;
        }
    }

}
// 6) (Optional) Logging Output
void WriteLogLine(struct logStruct* logStructPtrin)
{
    ;
}
// 7) (Optional) Config Input
void ReadConfigLine(struct configStruct* configStructPtrin)
{
    ;
}

void WritePacketBytes(struct packAPIStruct* uiStructPtrin)
{
    ;
}
void ReadPacketBytes(struct packAPIStruct* uiStructPtrin)
{
    ;
}

void readMotorData(struct smartMotorStruct* smartMotorStructPtrIn)
{
    ;
}
void writeMotorData(struct smartMotorStruct* smartMotorStructPtrIn)
{
    ;
}

///////////////////////////////////////////////////////////////////////
// Application Data Instances are Created here (Platform Specific)
ccGripperVersionsTemplate
#ifdef __cplusplus
theApplicationClass theApplicationExample;
#else
PLATFORM_APP_CTEMPLATE(Mn)
#endif

////////////////////////////////////////////////////////////////////////////////
// Platform Main Entry Points call ExeSys Area Functions
#ifdef MAIN_C_NOos_Wsystick
C_NOos_MAINnSYSTICK_TEMPLATE
#endif
#ifdef MAIN_C_NOos_NOsystick
C_OS_MAIN_TEMPLATE
#endif
