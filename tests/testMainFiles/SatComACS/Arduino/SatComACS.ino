/** \file SatComACS.ino
*   \brief part of <a href="https://github.com/InMechaSol/ccNOos">ccNOos</a>, Implementation for C++ wrappers 

    Copyright 2021 InMechaSol, Inc

    https://www.inmechasol.org/

    Licensed under the Apache License, Version 2.0(the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

Notes:
	(.c includes .h) - for straight C or
	(.cpp includes .c which includes .h) - for C++ wrapped straight C
	*Always compiled to a single compilation unit, either C or CPP, not both

*/

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
#if PLATFORM_NAME==Arduino
#include "Platform_Arduino.h"
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
    satcomacsStructPtrIn->ConsoleMenu.devptr = &ConsoleMenuserialdev.devdata;
    satcomacsStructPtrIn->LCDKeyPad.devptr = &LCDKeyPadserialdev.devdata;
    ConsoleMenuserialdev.devdata.triggerWriteOperation = ui8TRUE;
    satcomacsStructPtrIn->ConsoleMenu.showHelp = ui8TRUE;
    LCDKeyPadserialdev.devdata.triggerWriteOperation = ui8TRUE;
}
// 4) Basic ability for user console input via any io device
void GetMenuChars(struct uiStruct* uiStructPtrin)
{

    // if Consolue Menu
    if (uiStructPtrin->devptr == &ConsoleMenuserialdev.devdata)
    {
        if (consoleCharInt > -1)
        {
            if (uiStructPtrin->readIndex > 0 || (char)consoleCharInt != '\n')
            {
                uiStructPtrin->devptr->inbuff.charbuff[uiStructPtrin->readIndex] = (char)consoleCharInt;
                if (uiStructPtrin->devptr->inbuff.charbuff[uiStructPtrin->readIndex] == ';')
                {

                    uiStructPtrin->devptr->inbuff.charbuff[uiStructPtrin->readIndex + 1] = 0x00;
                    uiStructPtrin->devptr->newDataReadIn = ui8TRUE;
                    uiStructPtrin->parseIndex = 0;
                    uiStructPtrin->readIndex = 0;
                    return;
                }
                if (++uiStructPtrin->readIndex >= charBuffMax)
                    uiStructPtrin->readIndex = 0;
            }

        }
    }
    // if LCD KeyPad
    else if (uiStructPtrin->devptr == &LCDKeyPadserialdev.devdata)
    {
        ;
    }
}
// 5) Basic ability for user console output
void WriteMenuLine(struct uiStruct* uiStructPtrin)
{
    // if Consolue Menu
    if (uiStructPtrin->devptr == &ConsoleMenuserialdev.devdata)
    {
        if (uiStructPtrin->clearScreen) {
            Serial.print(terminalClearString());
            uiStructPtrin->clearScreen = ui8FALSE;
        }
        Serial.print(&uiStructPtrin->devptr->outbuff.charbuff[0]);
        if (uiStructPtrin->showPrompt) {
            Serial.print(terminalPromptString(uiStructPtrin->currentUserLevel));
            uiStructPtrin->showPrompt = ui8FALSE;
        }
    }
    // if LCD KeyPad
    else if (uiStructPtrin->devptr == &LCDKeyPadserialdev.devdata)
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



#define SETPIN_TX_C16(high_low)    IC1.digitalWrite(0, high_low)
#define SETPIN_TX_C8(high_low)     IC1.digitalWrite(1, high_low)
#define SETPIN_TX_C4(high_low)     IC1.digitalWrite(2, high_low)
#define SETPIN_TX_C2(high_low)     IC1.digitalWrite(3, high_low)
#define SETPIN_TX_C1(high_low)     IC1.digitalWrite(4, high_low)
#define SETPIN_TX_C0_25(high_low)  IC1.digitalWrite(5, high_low)
#define SETPIN_TX_C0_50(high_low)  IC1.digitalWrite(6, high_low)
#define SETPIN_TX_LE(high_low)     IC1.digitalWrite(14, high_low) 

#define SETPIN_RX_C16(high_low)    IC1.digitalWrite(7, high_low)
#define SETPIN_RX_C8(high_low)     IC1.digitalWrite(8, high_low)
#define SETPIN_RX_C4(high_low)     IC1.digitalWrite(9, high_low)
#define SETPIN_RX_C2(high_low)     IC1.digitalWrite(10, high_low)
#define SETPIN_RX_C1(high_low)     IC1.digitalWrite(11, high_low)
#define SETPIN_RX_C0_25(high_low)  IC1.digitalWrite(13, high_low)
#define SETPIN_RX_C0_50(high_low)  IC1.digitalWrite(12, high_low)
#define SETPIN_RX_LE(high_low)     IC1.digitalWrite(15, high_low) 

#define SETPIN_XX_C16(high_low)    
#define SETPIN_XX_C8(high_low)     
#define SETPIN_XX_C4(high_low)     
#define SETPIN_XX_C2(high_low)     
#define SETPIN_XX_C1(high_low)     
#define SETPIN_XX_C0_25(high_low)  
#define SETPIN_XX_C0_50(high_low)  
#define SETPIN_XX_LE(high_low)




float ModuloFloat(float floatValue, float* intPartPtr)
{
    double intPart, fracPart;
    fracPart = modf(floatValue, &intPart);
    *intPartPtr = (float)intPart;
    return (float)fracPart;
}

// Application Specific, Platform modifications
#define IC1Addr 0x27
bool runOnce = true;
Adafruit_MCP23017 IC1;
void writeAttenuatorValues(struct txRxStruct* txRxStructPtrIn) 
{ 
    if (runOnce)
    {
        runOnce = false;
        IC1.begin(IC1Addr);
        IC1.pinMode(0, OUTPUT);
        IC1.pinMode(1, OUTPUT);
        IC1.pinMode(2, OUTPUT);
        IC1.pinMode(3, OUTPUT);
        IC1.pinMode(4, OUTPUT);
        IC1.pinMode(5, OUTPUT);
        IC1.pinMode(6, OUTPUT);
        IC1.pinMode(7, OUTPUT);
        IC1.pinMode(8, OUTPUT);
        IC1.pinMode(9, OUTPUT);
        IC1.pinMode(10, OUTPUT);
        IC1.pinMode(11, OUTPUT);
        IC1.pinMode(12, OUTPUT);
        IC1.pinMode(13, OUTPUT);
        IC1.pinMode(14, OUTPUT);
        IC1.pinMode(15, OUTPUT);
    }

#define bit16   ( (0b10000000 & txRxStructPtrIn->CMD_AttenuatorBits) >> 7 )
#define bit8    ( (0b01000000 & txRxStructPtrIn->CMD_AttenuatorBits) >> 6 )
#define bit4    ( (0b00100000 & txRxStructPtrIn->CMD_AttenuatorBits) >> 5 )
#define bit2    ( (0b00010000 & txRxStructPtrIn->CMD_AttenuatorBits) >> 4 )
#define bit1    ( (0b00001000 & txRxStructPtrIn->CMD_AttenuatorBits) >> 3 )
#define bit0_25 ( (0b00000100 & txRxStructPtrIn->CMD_AttenuatorBits) >> 2 )
#define bit0_50 ( (0b00000010 & txRxStructPtrIn->CMD_AttenuatorBits) >> 1 )

    switch (txRxStructPtrIn->INDEX_Attenuator) {
    case 0:
        SETPIN_TX_C16(bit16);
        SETPIN_TX_C8(bit8);
        SETPIN_TX_C4(bit4);
        SETPIN_TX_C2(bit2);
        SETPIN_TX_C1(bit1);
        SETPIN_TX_C0_25(bit0_25);
        SETPIN_TX_C0_50(bit0_50);
        SETPIN_TX_LE(0x01);
        delay(1);
        SETPIN_TX_LE(0x00);
        break;
    case 1:
        SETPIN_RX_C16(bit16);
        SETPIN_RX_C8(bit8);
        SETPIN_RX_C4(bit4);
        SETPIN_RX_C2(bit2);
        SETPIN_RX_C1(bit1);
        SETPIN_RX_C0_25(bit0_25);
        SETPIN_RX_C0_50(bit0_50);
        SETPIN_RX_LE(0x01);
        delay(1);
        SETPIN_RX_LE(0x00);
        break;
    case 2:
        SETPIN_XX_C16(bit16);
        SETPIN_XX_C8(bit8);
        SETPIN_XX_C4(bit4);
        SETPIN_XX_C2(bit2);
        SETPIN_XX_C1(bit1);
        SETPIN_XX_C0_25(bit0_25);
        SETPIN_XX_C0_50(bit0_50);
        SETPIN_XX_LE(0x01);
        delay(1);
        SETPIN_XX_LE(0x00);
        break;
    default:
        break;
    }

#undef bit16  
#undef bit8
#undef bit4
#undef bit2
#undef bit1
#undef bit0_25 
#undef bit0_50  
}


UI_8 readGPS(struct gpsStruct* gpsStructPtrIn)
{
    int iNewLine = 0;
    if (gpsCharInt > -1)
    {

        GPSserialdev.devdata.inbuff.charbuff[GPSserialdev.readIndex] = (char)(gpsCharInt);
        if (GPSserialdev.devdata.inbuff.charbuff[GPSserialdev.readIndex] == '\n')
        {
            iNewLine = GPSserialdev.readIndex;
        }
        if (++GPSserialdev.readIndex >= charBuffMax)
            GPSserialdev.readIndex = 0;
    }

    //GPSserialdev.devdata.inbuff.charbuff[iNewLine] = '\0';

    if (iNewLine > 10)
    {
        gpsStructPtrIn->devptr = &GPSserialdev;
        GPSserialdev.readIndex = 0;
        return ui8TRUE;
    }
    else
        return ui8FALSE;

}


UI_8 readEcompass(struct eCompStruct* eCompStructPtrIn)
{
    int iNewLine = 0;
    if (eCompCharInt > -1)
    {
        eCompserialdev.devdata.inbuff.charbuff[eCompserialdev.readIndex] = (char)(eCompCharInt);
        if (eCompserialdev.devdata.inbuff.charbuff[eCompserialdev.readIndex] == '\n')
        {
            iNewLine = eCompserialdev.readIndex;
        }
        if (++eCompserialdev.readIndex >= charBuffMax)
            eCompserialdev.readIndex = 0;
    }

    //eCompserialdev.devdata.inbuff.charbuff[iNewLine] = '\0';

    if (iNewLine > 6)
    {
        eCompStructPtrIn->devptr = &eCompserialdev;
        eCompserialdev.readIndex = 0;
        return ui8TRUE;
    }
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

#ifdef MAIN_CPP_NOos_NOsystick
CPP_OS_MAIN_TEMPLATE
#endif

#ifdef MAIN_CPP_NOos_NOsystick_Arduino
CPP_MAIN_TEMPLATE_ARDUINO
#endif