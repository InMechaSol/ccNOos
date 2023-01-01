/** \file Application_Serialization.c
*   \brief <a href="https://www.inmechasol.org/" target="_blank">IMS</a>:
		<a href="https://github.com/InMechaSol/ccNOos" target="_blank">ccNOos</a>,
		Implementation for straight C 

    Copyright 2021 <a href="https://www.inmechasol.org/" target="_blank">InMechaSol, Inc</a>    

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
#include "Application_Serialization.h"

// Update a CRC accumulation with one data byte.
UI_16 ByteUpdateCrc(UI_16 crc, UI_8 data) {

    UI_16 x = (crc >> 8) ^ data;
    x ^= x >> 4;

    crc = (crc << 8) ^ (x << 12) ^ (x << 5) ^ x;
    return crc;
}
// Compute CRC word for a byte string.
UI_16 MakeCrc(const UI_8* data, UI_16 count) {

    UI_16 crc = 0xffff;

    UI_16 i;
    for (i = 0; i < count; i++) {
        crc = ByteUpdateCrc(crc, data[i]);
    }
    return crc;
}

// Individual Package Functions for the IQ Motors
void packageMultiTurnPosRequest(struct smartMotorStruct* smartMotorStructPtrIn)
{
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[0] = PREAMBLE;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[1] = 0x2;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[2] = TYPE_MULTITURN;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[3] = SUBTYPE_OBSANGDISP;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[4] = (smartMotorStructPtrIn->ModuleID << 2) | (ACCESS_GET);
    UI_16 thisCrc = MakeCrc(&smartMotorStructPtrIn->devptr->outbuff.bytebuff[1], 4);
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[5] = thisCrc & 0x00FF;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[6] = (thisCrc & 0xFF00) >> 8;
    smartMotorStructPtrIn->devptr->numbytes2Write = 7;
}
void packageMultiTurnVelRequest(struct smartMotorStruct* smartMotorStructPtrIn)
{
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[0] = PREAMBLE;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[1] = 0x2;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[2] = TYPE_MULTITURN;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[3] = SUBTYPE_OBSANGVEL;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[4] = (smartMotorStructPtrIn->ModuleID << 2) | (ACCESS_GET);
    UI_16 thisCrc = MakeCrc(&smartMotorStructPtrIn->devptr->outbuff.bytebuff[1], 4);
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[5] = thisCrc & 0x00FF;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[6] = (thisCrc & 0xFF00) >> 8;
    smartMotorStructPtrIn->devptr->numbytes2Write = 7;
}
void packageBrshDrvMotorCurrentRequest(struct smartMotorStruct* smartMotorStructPtrIn)
{
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[0] = PREAMBLE;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[1] = 0x2;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[2] = TYPE_BRUSHLESSDRV;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[3] = SUBTYPE_ESTCUR;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[4] = (smartMotorStructPtrIn->ModuleID << 2) | (ACCESS_GET);
    UI_16 thisCrc = MakeCrc(&smartMotorStructPtrIn->devptr->outbuff.bytebuff[1], 4);
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[5] = thisCrc & 0x00FF;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[6] = (thisCrc & 0xFF00) >> 8;
    smartMotorStructPtrIn->devptr->numbytes2Write = 7;
}
void packageTempEstMotorTempRequest(struct smartMotorStruct* smartMotorStructPtrIn)
{
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[0] = PREAMBLE;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[1] = 0x2;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[2] = TYPE_TEMPEST;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[3] = SUBTYPE_TEMP;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[4] = (smartMotorStructPtrIn->ModuleID << 2) | (ACCESS_GET);
    UI_16 thisCrc = MakeCrc(&smartMotorStructPtrIn->devptr->outbuff.bytebuff[1], 4);
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[5] = thisCrc & 0x00FF;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[6] = (thisCrc & 0xFF00) >> 8;
    smartMotorStructPtrIn->devptr->numbytes2Write = 7;
}
void packageBrshDrvSetCurrentLimit(struct smartMotorStruct* smartMotorStructPtrIn)
{
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[0] = PREAMBLE;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[1] = 0x6;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[2] = TYPE_BRUSHLESSDRV;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[3] = SUBTYPE_ILIMIT;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[4] = (smartMotorStructPtrIn->ModuleID << 2) | (ACCESS_SET);
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[5] = ((UI_8*)&smartMotorStructPtrIn->MotorCurrentLimitCMD)[0];
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[6] = ((UI_8*)&smartMotorStructPtrIn->MotorCurrentLimitCMD)[1];
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[7] = ((UI_8*)&smartMotorStructPtrIn->MotorCurrentLimitCMD)[2];
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[8] = ((UI_8*)&smartMotorStructPtrIn->MotorCurrentLimitCMD)[3];
    UI_16 thisCrc = MakeCrc(&smartMotorStructPtrIn->devptr->outbuff.bytebuff[1], 8);
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[9] = thisCrc & 0x00FF;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[10] = (thisCrc & 0xFF00) >> 8;
    smartMotorStructPtrIn->devptr->numbytes2Write = 11;
}
void packageBrshDrvSetPWMCmd(struct smartMotorStruct* smartMotorStructPtrIn)
{
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[0] = PREAMBLE;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[1] = 0x6;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[2] = TYPE_BRUSHLESSDRV;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[3] = SUBTYPE_DRVPWM;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[4] = (smartMotorStructPtrIn->ModuleID << 2) | (ACCESS_SET);
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[5] = ((UI_8*)&smartMotorStructPtrIn->MotorPWMCmd)[0];
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[6] = ((UI_8*)&smartMotorStructPtrIn->MotorPWMCmd)[1];
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[7] = ((UI_8*)&smartMotorStructPtrIn->MotorPWMCmd)[2];
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[8] = ((UI_8*)&smartMotorStructPtrIn->MotorPWMCmd)[3];
    UI_16 thisCrc = MakeCrc(&smartMotorStructPtrIn->devptr->outbuff.bytebuff[1], 8);
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[9] = thisCrc & 0x00FF;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[10] = (thisCrc & 0xFF00) >> 8;
    smartMotorStructPtrIn->devptr->numbytes2Write = 11;
}
void packageBrshDrvPWMCmdRequest(struct smartMotorStruct* smartMotorStructPtrIn)
{
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[0] = PREAMBLE;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[1] = 0x2;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[2] = TYPE_BRUSHLESSDRV;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[3] = SUBTYPE_DRVPWM;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[4] = (smartMotorStructPtrIn->ModuleID << 2) | (ACCESS_GET);
    UI_16 thisCrc = MakeCrc(&smartMotorStructPtrIn->devptr->outbuff.bytebuff[1], 4);
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[5] = thisCrc & 0x00FF;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[6] = (thisCrc & 0xFF00) >> 8;
    smartMotorStructPtrIn->devptr->numbytes2Write = 7;
}
void packageBrshDrvOperateModeRequest(struct smartMotorStruct* smartMotorStructPtrIn)
{
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[0] = PREAMBLE;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[1] = 0x2;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[2] = TYPE_BRUSHLESSDRV;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[3] = SUBTYPE_DRVMODE;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[4] = (smartMotorStructPtrIn->ModuleID << 2) | (ACCESS_GET);
    UI_16 thisCrc = MakeCrc(&smartMotorStructPtrIn->devptr->outbuff.bytebuff[1], 4);
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[5] = thisCrc & 0x00FF;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[6] = (thisCrc & 0xFF00) >> 8;
    smartMotorStructPtrIn->devptr->numbytes2Write = 7;
}
void packageBrshDrvSetCoastMode(struct smartMotorStruct* smartMotorStructPtrIn)
{
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[0] = PREAMBLE;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[1] = 0x2;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[2] = TYPE_BRUSHLESSDRV;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[3] = SUBTYPE_DRVCOAST;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[4] = (smartMotorStructPtrIn->ModuleID << 2) | (ACCESS_SET);
    UI_16 thisCrc = MakeCrc(&smartMotorStructPtrIn->devptr->outbuff.bytebuff[1], 8);
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[5] = thisCrc & 0x00FF;
    smartMotorStructPtrIn->devptr->outbuff.bytebuff[6] = (thisCrc & 0xFF00) >> 8;
    smartMotorStructPtrIn->devptr->numbytes2Write = 7;
}

// Individual Parse Functions for the IQ Motors
void parseMultiTurnPosReply(struct smartMotorStruct* smartMotorStructPtrIn)
{
    if (((smartMotorStructPtrIn->devptr->inbuff.bytebuff[4] & 0x03) == 0x03) && (smartMotorStructPtrIn->devptr->inbuff.bytebuff[3] == SUBTYPE_OBSANGDISP))
    {
        smartMotorStructPtrIn->Connected = ui8TRUE;

        *(((UI_8*)(&smartMotorStructPtrIn->RotorPositionFbk)) + 0) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[5];
        *(((UI_8*)(&smartMotorStructPtrIn->RotorPositionFbk)) + 1) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[6];
        *(((UI_8*)(&smartMotorStructPtrIn->RotorPositionFbk)) + 2) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[7];
        *(((UI_8*)(&smartMotorStructPtrIn->RotorPositionFbk)) + 3) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[8];
    }
    else
        smartMotorStructPtrIn->Connected = ui8FALSE;
}
void parseMultiTurnVelReply(struct smartMotorStruct* smartMotorStructPtrIn)
{
    if (((smartMotorStructPtrIn->devptr->inbuff.bytebuff[4] & 0x03) == 0x03) && (smartMotorStructPtrIn->devptr->inbuff.bytebuff[3] == SUBTYPE_OBSANGVEL))
    {
        *(((UI_8*)(&smartMotorStructPtrIn->RotorVelocityFbk)) + 0) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[5];
        *(((UI_8*)(&smartMotorStructPtrIn->RotorVelocityFbk)) + 1) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[6];
        *(((UI_8*)(&smartMotorStructPtrIn->RotorVelocityFbk)) + 2) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[7];
        *(((UI_8*)(&smartMotorStructPtrIn->RotorVelocityFbk)) + 3) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[8];
    }
    else
        smartMotorStructPtrIn->Connected = ui8FALSE;
}
void parseBrshDrvCurrentReply(struct smartMotorStruct* smartMotorStructPtrIn)
{
    if (((smartMotorStructPtrIn->devptr->inbuff.bytebuff[4] & 0x03) == 0x03) && (smartMotorStructPtrIn->devptr->inbuff.bytebuff[3] == SUBTYPE_ESTCUR))
    {
        *(((UI_8*)(&smartMotorStructPtrIn->MotorCurrentFbk)) + 0) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[5];
        *(((UI_8*)(&smartMotorStructPtrIn->MotorCurrentFbk)) + 1) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[6];
        *(((UI_8*)(&smartMotorStructPtrIn->MotorCurrentFbk)) + 2) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[7];
        *(((UI_8*)(&smartMotorStructPtrIn->MotorCurrentFbk)) + 3) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[8];
    }
    else
        smartMotorStructPtrIn->Connected = ui8FALSE;
}
void parseTempEstMotorTemptReply(struct smartMotorStruct* smartMotorStructPtrIn)
{
    if (((smartMotorStructPtrIn->devptr->inbuff.bytebuff[4] & 0x03) == ACCESS_REPLY) && 
        (smartMotorStructPtrIn->devptr->inbuff.bytebuff[3] == SUBTYPE_TEMP))
    {
        *(((UI_8*)(&smartMotorStructPtrIn->MotorTempDegC)) + 0) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[5];
        *(((UI_8*)(&smartMotorStructPtrIn->MotorTempDegC)) + 1) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[6];
        *(((UI_8*)(&smartMotorStructPtrIn->MotorTempDegC)) + 2) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[7];
        *(((UI_8*)(&smartMotorStructPtrIn->MotorTempDegC)) + 3) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[8];
    }
    else
        smartMotorStructPtrIn->Connected = ui8FALSE;
}
void parseBrshDrvOperateModeReply(struct smartMotorStruct* smartMotorStructPtrIn)
{
    if (((smartMotorStructPtrIn->devptr->inbuff.bytebuff[4] & 0x03) == ACCESS_REPLY) && 
        (smartMotorStructPtrIn->devptr->inbuff.bytebuff[3] == SUBTYPE_DRVMODE))
    {
        smartMotorStructPtrIn->MotorControlModeFbk = (enum brshDriveCtrlMode)smartMotorStructPtrIn->devptr->inbuff.bytebuff[5];
    }
    else
        smartMotorStructPtrIn->Connected = ui8FALSE;
}
void parseBrshDrvPWMCmdReply(struct smartMotorStruct* smartMotorStructPtrIn)
{
    if (((smartMotorStructPtrIn->devptr->inbuff.bytebuff[4] & 0x03) == ACCESS_REPLY) &&
        (smartMotorStructPtrIn->devptr->inbuff.bytebuff[3] == SUBTYPE_DRVPWM))
    {
        *(((UI_8*)(&smartMotorStructPtrIn->MotorPWMCmdReturned)) + 0) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[5];
        *(((UI_8*)(&smartMotorStructPtrIn->MotorPWMCmdReturned)) + 1) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[6];
        *(((UI_8*)(&smartMotorStructPtrIn->MotorPWMCmdReturned)) + 2) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[7];
        *(((UI_8*)(&smartMotorStructPtrIn->MotorPWMCmdReturned)) + 3) = smartMotorStructPtrIn->devptr->inbuff.bytebuff[8];
    }
    else
        smartMotorStructPtrIn->Connected = ui8FALSE;
}

// Cummulative Parse and Package Functions for the IQ Smart Motor Device
void parseMotorData(struct smartMotorStruct* smartMotorStructPtrIn)
{
    switch (smartMotorStructPtrIn->MotorsCommPhase)
    {
    case 1:// parse position reply
        parseMultiTurnPosReply(smartMotorStructPtrIn); break;
    case 3:// parse velocity reply
        parseMultiTurnVelReply(smartMotorStructPtrIn); break;
    case 5:// parse current reply
        parseBrshDrvCurrentReply(smartMotorStructPtrIn); break;
    case 7:// parse temperature or PWMCmd reply
        if (smartMotorStructPtrIn->MotorsCommSubPhase == ui8FALSE)
        {
            parseTempEstMotorTemptReply(smartMotorStructPtrIn);
            smartMotorStructPtrIn->MotorsCommSubPhase = ui8TRUE;
        }
        else
        {
            parseTempEstMotorTemptReply(smartMotorStructPtrIn);
            smartMotorStructPtrIn->MotorsCommSubPhase = ui8FALSE;
        }
    break;
    default: break;
    }
}
void packageMotorData(struct smartMotorStruct* smartMotorStructPtrIn)
{    
    switch (smartMotorStructPtrIn->MotorsCommPhase)
    {
    case 0:// package postion request
        packageMultiTurnPosRequest(smartMotorStructPtrIn); break;
    case 2:// package velocity request
        packageMultiTurnVelRequest(smartMotorStructPtrIn); break;
    case 4:// package current request
        packageBrshDrvMotorCurrentRequest(smartMotorStructPtrIn); break;
    case 6:// package temperature request or PWMCmd request
        if(smartMotorStructPtrIn->MotorsCommSubPhase == ui8FALSE)
            packageTempEstMotorTempRequest(smartMotorStructPtrIn);
        else 
            packageTempEstMotorTempRequest(smartMotorStructPtrIn);
    break;



    case 8:// package current limit cmd, if it has changed;
        packageBrshDrvSetCurrentLimit(smartMotorStructPtrIn);   break;
    case 9:// package set coast mode or pwm cmd
        packageBrshDrvSetPWMCmd(smartMotorStructPtrIn);
        //else
            //packageBrshDrvSetCoastMode(smartMotorStructPtrIn);
    break;
    default: break;
    }
}

