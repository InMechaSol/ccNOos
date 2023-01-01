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
#if PLATFORM_NAME==Arduino
#include "Platform_Teensy41.hpp"
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

struct devicedatastruct PacketsInterfactDevDataStruct = createDeviceStruct();
struct devicedatastruct ConsoleMenuDevDataStruct = createDeviceStruct();
struct devicedatastruct SmartMotorsDevDataStruct[NUMMOTORS];

///////////////////////////////////////////////////////////////////////
// Platform and Application Specific IO Device Functions
void linkAPIioDevices(struct ccGripperStruct* gripperStructPtrIn)
{
    
    int i;
    for (i = 0; i < NUMMOTORS; i++)
        gripperStructPtrIn->SmartMotors[i].devptr = &SmartMotorsDevDataStruct[i];
    ConsoleMenuDevDataStruct.numbytes2Read = 1;
    ConsoleMenuDevDataStruct.triggerWriteOperation = ui8TRUE;
    gripperStructPtrIn->ConsoleMenu.devptr = &ConsoleMenuDevDataStruct;
    gripperStructPtrIn->PacketsAPI.devptr = &PacketsInterfactDevDataStruct;    
    gripperStructPtrIn->ConsoleMenu.showHelp = ui8TRUE;
#ifdef EVENODDMOTORS
    gripperStructPtrIn->SmartMotors[1].ModuleID = 1;
    gripperStructPtrIn->SmartMotors[3].ModuleID = 1;
#endif
}

UI_32 getMillis() { return millis(); }

//////////////////////////////////////////////////////////////////
// Device Module IO Functions
void readMotorData(struct smartMotorStruct* smartMotorStructPtrIn)
{
    if (smartMotorStructPtrIn->devptr == &SmartMotorsDevDataStruct[0])
    {
        if (motor0CharInt > -1)
        {
            smartMotorStructPtrIn->devptr->inbuff.bytebuff[smartMotorStructPtrIn->devptr->numbytesReadIn++] = (UI_8)motor0CharInt;
        }
    }
    else if (smartMotorStructPtrIn->devptr == &SmartMotorsDevDataStruct[1])
    {
        if (motor1CharInt > -1)
        {
            smartMotorStructPtrIn->devptr->inbuff.bytebuff[smartMotorStructPtrIn->devptr->numbytesReadIn++] = (UI_8)motor1CharInt;
        }
    }
    else if (smartMotorStructPtrIn->devptr == &SmartMotorsDevDataStruct[2])
    {
        if (motor2CharInt > -1)
        {
            smartMotorStructPtrIn->devptr->inbuff.bytebuff[smartMotorStructPtrIn->devptr->numbytesReadIn++] = (UI_8)motor2CharInt;
        }
    }
    else if (smartMotorStructPtrIn->devptr == &SmartMotorsDevDataStruct[3])
    {
        if (motor3CharInt > -1)
        {
            smartMotorStructPtrIn->devptr->inbuff.bytebuff[smartMotorStructPtrIn->devptr->numbytesReadIn++] = (UI_8)motor3CharInt;
        }
    }

    if (smartMotorStructPtrIn->devptr->numbytesReadIn > 1)
    {
        if (smartMotorStructPtrIn->devptr->inbuff.bytebuff[1] + 5 == smartMotorStructPtrIn->devptr->numbytesReadIn)
        {
            smartMotorStructPtrIn->devptr->newDataReadIn = ui8TRUE;
        }
    }
}
void writeMotorData(struct smartMotorStruct* smartMotorStructPtrIn)
{
    if (smartMotorStructPtrIn->devptr == &SmartMotorsDevDataStruct[0])
    {
        smartMotorStructPtrIn->devptr->numbytesWritten = Serial1.write(&smartMotorStructPtrIn->devptr->outbuff.bytebuff[0],smartMotorStructPtrIn->devptr->numbytes2Write);
    }
    else if (smartMotorStructPtrIn->devptr == &SmartMotorsDevDataStruct[1])
    {
        smartMotorStructPtrIn->devptr->numbytesWritten = Serial2.write(&smartMotorStructPtrIn->devptr->outbuff.bytebuff[0], smartMotorStructPtrIn->devptr->numbytes2Write);
    }
    else if (smartMotorStructPtrIn->devptr == &SmartMotorsDevDataStruct[2])
    {
        smartMotorStructPtrIn->devptr->numbytesWritten = Serial3.write(&smartMotorStructPtrIn->devptr->outbuff.bytebuff[0], smartMotorStructPtrIn->devptr->numbytes2Write);
    }
    else if (smartMotorStructPtrIn->devptr == &SmartMotorsDevDataStruct[3])
    {
        smartMotorStructPtrIn->devptr->numbytesWritten = Serial4.write(&smartMotorStructPtrIn->devptr->outbuff.bytebuff[0], smartMotorStructPtrIn->devptr->numbytes2Write);
    }

}


/////////////////////////////////////////////////////////////////
// API Module IO Funcitions
// 4) Basic ability for user console input via any io device
void GetMenuChars(struct uiStruct* uiStructPtrin)
{
    // if Consolue Menu
    if (uiStructPtrin->devptr == &ConsoleMenuDevDataStruct)
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
}
// 5) Basic ability for user console output
void WriteMenuLine(struct uiStruct* uiStructPtrin)
{
    // if Consolue Menu
    if (uiStructPtrin->devptr == &ConsoleMenuDevDataStruct)
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

}


void WritePacketBytes(struct packAPIStruct* packStructPtrin)
{
    packStructPtrin->devptr->numbytesWritten = Udp.beginPacket(Udp.remoteIP(), C_DEST_UDP_PORT);
    packStructPtrin->devptr->numbytesWritten += Udp.write(&packStructPtrin->devptr->outbuff.bytebuff[0], packStructPtrin->devptr->numbytes2Write);
    packStructPtrin->devptr->numbytesWritten += Udp.endPacket();
    packStructPtrin->outPackCounter++;
    packStructPtrin->outPackBytesCounter += packStructPtrin->devptr->numbytes2Write;
}
void ReadPacketBytes(struct packAPIStruct* packStructPtrin)
{
    // !!! This function calls a tight loop that could hang forever...
    udpByteInt = Udp.parsePacket();
    if (udpByteInt > 4)
    {        
        packStructPtrin->devptr->numbytesReadIn = Udp.read(&packStructPtrin->devptr->inbuff.bytebuff[0], udpByteInt);
    }
    // If at least the header length in bytes has been readin
    if (packStructPtrin->devptr->numbytesReadIn > 4)
    {
        
        // If the expected length of packets has been readin
        if (packStructPtrin->devptr->numbytesReadIn == packStructPtrin->devptr->inbuff.bytebuff[3])
        {
            // If the expected length is at both header and tail locations
            if (packStructPtrin->devptr->inbuff.bytebuff[packStructPtrin->devptr->numbytesReadIn - 1] ==
                packStructPtrin->devptr->inbuff.bytebuff[3])
            {
                // Its a packet, parse it
                packStructPtrin->devptr->parseIndex = 0;
                packStructPtrin->devptr->newDataReadIn = ui8TRUE;
                packStructPtrin->inPackCounter++;
                packStructPtrin->inPackBytesCounter += packStructPtrin->devptr->numbytesReadIn;
            }
        }  
    }
    packStructPtrin->devptr->numbytesReadIn = 0;
    
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
