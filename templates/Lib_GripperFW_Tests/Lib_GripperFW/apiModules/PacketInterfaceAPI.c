/** \file PacketInterfaceAPI.c
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
#include "PacketInterfaceAPI.h"


void parseGripperPacketsAPI(struct ccGripperStruct* gripperStructPtrIn, struct packAPIStruct* packStructPtrIn)
{
    int i;

    // parse the packet header
    parseHeader(packStructPtrIn);

    // based on header, parse packet
    if (packStructPtrIn->inPackID == 0)
    {
#ifdef FWCONTROLCLIENT
        parseToken(packStructPtrIn, &gripperStructPtrIn->GripperMode[0], sizeof(UI_8));
        parseToken(packStructPtrIn, &gripperStructPtrIn->VacuumExtend.VacuumSensorADC, sizeof(UI_16));
        parseToken(packStructPtrIn, &gripperStructPtrIn->TimeStamp, sizeof(UI_32));
        for (i = 0; i < NUMMOTORS; i++)
        {
            parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[i].RotorPositionFbk, sizeof(float));
            parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[i].RotorVelocityFbk, sizeof(float));
            parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[i].MotorCurrentFbk, sizeof(float));
            parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[i].MotorTempDegC, sizeof(float));
            parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[i].MotorPWMCmd, sizeof(float));
            parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[i].StatusOK, sizeof(UI_8));
            parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[i].Connected, sizeof(UI_8));
            parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[i].MotionState, sizeof(UI_8));
        }
        // each cycle, parse some of the "slow" data
        parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->inPackType].RotorPositionHighStop, sizeof(float));
        parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->inPackType].RotorPositionLowStop, sizeof(float));
        parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->inPackType].RotorPositionFbkStart, sizeof(float));
        parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->inPackType].RotorPositionFbkStop, sizeof(float));
        parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->inPackType].MotionRange, sizeof(float));
        parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->inPackType].MotionRangeLimit, sizeof(float));
        parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->inPackType].CommPeriodAbsoluteMaximum, sizeof(UI_32));
        parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->inPackType].CommPeriodAbsoluteMinimum, sizeof(UI_32));
        parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->inPackType].CommPeriodAverage, sizeof(UI_32));
        parseToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->inPackType].millisThentoNow, sizeof(UI_32));
        
#endif
#ifndef FWCONTROLCLIENT

#endif
        packStructPtrIn->Connected = ui8TRUE;
    }
    else if (packStructPtrIn->inPackID == 1)
    {
        // open
        gripperStructPtrIn->TriggerOpen = ui8TRUE;
    }
    else if (packStructPtrIn->inPackID == 2)
    {
        // close
        gripperStructPtrIn->TriggerClose = ui8TRUE;
    }
    else if (packStructPtrIn->inPackID == 3)
    {
        // latch
        gripperStructPtrIn->TriggerLatch = ui8TRUE;
    }
    else if (packStructPtrIn->inPackID == 4)
    {
        // stop
        gripperStructPtrIn->TriggerStop = ui8TRUE;
    }
}



void packageGripperPacketsAPI(struct ccGripperStruct* gripperStructPtrIn, struct packAPIStruct* packStructPtrIn)
{
    int i;

#ifndef FWCONTROLCLIENT
    // set desired packet header, increment n
    packStructPtrIn->outPackID = 0;
    packStructPtrIn->outPackType = (++packStructPtrIn->outPackType % NUMMOTORS);
    
    // package header
    packageHeader(packStructPtrIn);
    
    // each cycle, package the "fast" data
    packageToken(packStructPtrIn, &gripperStructPtrIn->GripperMode[0], sizeof(UI_8));
    packageToken(packStructPtrIn, &gripperStructPtrIn->VacuumExtend.VacuumSensorADC, sizeof(UI_16));
    packageToken(packStructPtrIn, &gripperStructPtrIn->TimeStamp, sizeof(UI_32));
    for (i = 0; i < NUMMOTORS; i++)
    {
        packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[i].RotorPositionFbk, sizeof(float));
        packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[i].RotorVelocityFbk, sizeof(float));
        packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[i].MotorCurrentFbk, sizeof(float));
        packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[i].MotorTempDegC, sizeof(float));
        packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[i].MotorPWMCmd, sizeof(float));
        packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[i].StatusOK, sizeof(UI_8));
        packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[i].Connected, sizeof(UI_8));
        packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[i].MotionState, sizeof(UI_8));
    }

    // each cycle, package some of the "slow" data
    packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->outPackType].RotorPositionHighStop, sizeof(float));
    packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->outPackType].RotorPositionLowStop, sizeof(float));
    packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->outPackType].RotorPositionFbkStart, sizeof(float));
    packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->outPackType].RotorPositionFbkStop, sizeof(float));
    packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->outPackType].MotionRange, sizeof(float));
    packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->outPackType].MotionRangeLimit, sizeof(float));
    packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->outPackType].CommPeriodAbsoluteMaximum, sizeof(UI_32));
    packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->outPackType].CommPeriodAbsoluteMinimum, sizeof(UI_32));
    packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->outPackType].CommPeriodAverage, sizeof(UI_32));
    packageToken(packStructPtrIn, &gripperStructPtrIn->SmartMotors[packStructPtrIn->outPackType].millisThentoNow, sizeof(UI_32));
    


    // set length to numbytes2write
    packStructPtrIn->devptr->outbuff.bytebuff[packStructPtrIn->devptr->numbytes2Write++] = (UI_8)packStructPtrIn->devptr->numbytes2Write;
    packStructPtrIn->devptr->outbuff.bytebuff[3] = (UI_8)packStructPtrIn->devptr->numbytes2Write;
    packStructPtrIn->outPackLen = (UI_8)packStructPtrIn->devptr->numbytes2Write;

#endif
#ifdef FWCONTROLCLIENT


    packStructPtrIn->devptr->numbytes2Write = 6;
    // first byte
    packStructPtrIn->devptr->outbuff.bytebuff[0] = 0;   // pNUM
    packStructPtrIn->devptr->outbuff.bytebuff[1] = 0;   // pID
    packStructPtrIn->devptr->outbuff.bytebuff[2] = 0;   // pTYPE
    packStructPtrIn->devptr->outbuff.bytebuff[3] = packStructPtrIn->devptr->numbytes2Write;

    // Package CMD Byte
    if(gripperStructPtrIn->PackageCMDOpen)
    {
        gripperStructPtrIn->PackageCMDOpen = ui8FALSE;
        packStructPtrIn->devptr->outbuff.bytebuff[1] = 1;// pID
    }
    else if(gripperStructPtrIn->PackageCMDClose)
    {
        gripperStructPtrIn->PackageCMDClose = ui8FALSE;
        packStructPtrIn->devptr->outbuff.bytebuff[1] = 2;// pID
    }
    else if(gripperStructPtrIn->PackageCMDLatch)
    {
        gripperStructPtrIn->PackageCMDLatch = ui8FALSE;
        packStructPtrIn->devptr->outbuff.bytebuff[1] = 3;// pID
    }
    else if(gripperStructPtrIn->PackageCMDStop)
    {
        gripperStructPtrIn->PackageCMDStop = ui8FALSE;
        packStructPtrIn->devptr->outbuff.bytebuff[1] = 4;// pID
    }


    // last byte
    packStructPtrIn->devptr->outbuff.bytebuff[packStructPtrIn->devptr->numbytes2Write-1] = packStructPtrIn->devptr->numbytes2Write;
    packStructPtrIn->devptr->numbytesWritten = 0;

#endif
}

