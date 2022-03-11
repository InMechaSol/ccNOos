/** \file WIN32_Console.cpp
*   \brief WIN32 Console Implementation of ccNOos_Tests in c++

   Copyright 2021 InMechaSol, Inc

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

Notes:

*/
//////////////////////////////////
// Include Application and cNOos
#include "PlatformApp_Serialization.h"

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
#include "Platform_Win32.h"
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




///////////////////////////////////////////////////////////////////////
// Platform and Application Specific IO Device Functions
void linkAPIioDevices(struct SatComACSStruct* satcomacsStructPtrIn)
{
    satcomacsStructPtrIn->ConsoleMenu.devptr = &ConsoleMenuDevDataStruct;
    satcomacsStructPtrIn->LCDKeyPad.devptr = &LCDKeyPadDevDataStruct;
    ConsoleMenuDevDataStruct.triggerWriteOperation = ui8TRUE;
    satcomacsStructPtrIn->ConsoleMenu.showHelp = ui8TRUE;
    LCDKeyPadDevDataStruct.triggerWriteOperation = ui8TRUE;
}
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
        if (stdInThreadRunning == ui8FALSE )
        {
            if (runONCE)
            {
                stdInThread = std::thread(readStdIn, &uiStructPtrin->devptr->inbuff.charbuff[0]);
                runONCE = false;
                stdInThreadRunning = ui8TRUE;
            }
            else if(uiStructPtrin->devptr->triggerWriteOperation == ui8FALSE)
            {
                uiStructPtrin->devptr->newDataReadIn = ui8TRUE;                
                uiStructPtrin->parseIndex = 0;
                stdInThreadRunning = ui8TRUE;
            } 
        }
    }
    // if LCD KeyPad
    else if (uiStructPtrin->devptr == &LCDKeyPadDevDataStruct)
    {
        ;
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
    // if LCD KeyPad
    else if (uiStructPtrin->devptr == &LCDKeyPadDevDataStruct)
    {
        ;
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


#define SETPIN_TX_C16(high_low)    
#define SETPIN_TX_C8(high_low)     
#define SETPIN_TX_C4(high_low)     
#define SETPIN_TX_C2(high_low)     
#define SETPIN_TX_C1(high_low)     
#define SETPIN_TX_C0_25(high_low)  
#define SETPIN_TX_C0_50(high_low)  
#define SETPIN_TX_LE(high_low)     

#define SETPIN_RX_C16(high_low)    
#define SETPIN_RX_C8(high_low)     
#define SETPIN_RX_C4(high_low)     
#define SETPIN_RX_C2(high_low)     
#define SETPIN_RX_C1(high_low)     
#define SETPIN_RX_C0_25(high_low)  
#define SETPIN_RX_C0_50(high_low)  
#define SETPIN_RX_LE(high_low)     

#define SETPIN_XX_C16(high_low)    
#define SETPIN_XX_C8(high_low)     
#define SETPIN_XX_C4(high_low)     
#define SETPIN_XX_C2(high_low)     
#define SETPIN_XX_C1(high_low)     
#define SETPIN_XX_C0_25(high_low)  
#define SETPIN_XX_C0_50(high_low)  
#define SETPIN_XX_LE(high_low)     

#define SETPIN_MDM1_SW(high_low)   
#define SETPIN_MDM2_SW(high_low)   
#define SETPIN_MDM3_SW(high_low)   
#define SETPIN_MDM4_SW(high_low)   

SatComACS_TxRx_WriteFuncsTemplate(std::this_thread::sleep_for(std::chrono::milliseconds(1)))

UI_8 readGPS(struct gpsStruct* gpsStructPtrIn)
{
    GPSPortParams.serialdev.numBytes2Read = 1;
    GPSPortParams.serialdev.readIndex = 0;
    gpsStructPtrIn->devptr = &GPSPortParams.serialdev;
    int iNewLine = 0;
    int tries = 0;
    do {


        if (GPS_NBSerial.ReadDev() == GPSPortParams.serialdev.numBytes2Read)
        {
            if (GPSPortParams.serialdev.devdata.inbuff.charbuff[GPSPortParams.serialdev.readIndex] == '\n')
            {
                iNewLine = GPSPortParams.serialdev.readIndex;
            }
            GPSPortParams.serialdev.readIndex += GPSPortParams.serialdev.numBytes2Read;
        }
        else if (GPSPortParams.serialdev.readIndex > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            tries++;
        }
        else
            break;


    } while (GPSPortParams.serialdev.readIndex < charBuffMax && iNewLine == 0 && tries < 5);

    GPSPortParams.serialdev.devdata.inbuff.charbuff[iNewLine] = '\0';

    if (iNewLine > 6)
        return ui8TRUE;
    else
        return ui8FALSE;
}
UI_8 readEcompass(struct eCompStruct* eCompStructPtrIn)
{
    eCompPortParams.serialdev.numBytes2Read = 1;
    eCompPortParams.serialdev.readIndex = 0;
    eCompStructPtrIn->devptr = &eCompPortParams.serialdev;
    int iNewLine = 0;
    int tries = 0;
    do {

        
        if (eComp_NBSerial.ReadDev() == eCompPortParams.serialdev.numBytes2Read)
        {
            if (eCompPortParams.serialdev.devdata.inbuff.charbuff[eCompPortParams.serialdev.readIndex] == '\n')
            {
                iNewLine = eCompPortParams.serialdev.readIndex;
            }
            eCompPortParams.serialdev.readIndex += eCompPortParams.serialdev.numBytes2Read;
        }
        else if (eCompPortParams.serialdev.readIndex > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            tries++;
        }
        else
            break;


    } while (eCompPortParams.serialdev.readIndex < charBuffMax && iNewLine == 0 && tries < 5);

    eCompPortParams.serialdev.devdata.inbuff.charbuff[iNewLine] = '\0';

    if (iNewLine > 10)
        return ui8TRUE;
    else
        return ui8FALSE;
}



void readFreqConv(struct freqConvStruct* freqConvStructPtrIn) { ; }
void writeFreqConv(struct freqConvStruct* freqConvStructPtrIn) { ; }
void readPowerMeter(struct powerMeterStruct* powerMeterStructPtrIn) { ; }
void writePowerMeter(struct powerMeterStruct* powerMeterStructPtrIn) { ; }


///////////////////////////////////////////////////////////////////////
// Application Data Instances are Created here (Platform Specific)
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
