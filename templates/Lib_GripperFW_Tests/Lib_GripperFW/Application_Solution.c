/** \file Application_Solution.c
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
#include "Application_Solution.h"



// Re-usable, portable, cross-platform create() function
MODdeclareCREATE(Mn)(MODdeclareCREATEINS)
{
    MODdeclareSTRUCT(Mn) outStruct;
    int i;
    outStruct.compMod = CreateComputeModuleStruct();    
    outStruct.ConsoleMenu = createuiStruct();
    outStruct.PacketsAPI = createpackAPIStruct();
    outStruct.GripperMode[iCURRENT] = gripper_uninitialized;
    outStruct.GripperMode[iCURRENT] = gripper_uninitialized;
    outStruct.AutoUpdate = ui8FALSE;
    for( i = 0; i < NUMMOTORS; i++)
        outStruct.SmartMotors[i] = createsmartMotorStruct();
    outStruct.execAPIsMainLoop = ui8TRUE;
    outStruct.execDEVsMainLoop = ui8TRUE;
    for (i = 0; i < 20; i++)
        outStruct.OperationsLog[i] = 0;
    outStruct.OpsLogIndex = 0;
    outStruct.OpsLogSize = 0;
    outStruct.TriggerClose = ui8FALSE;
    outStruct.TriggerOpen = ui8FALSE;
    outStruct.TriggerLatch = ui8FALSE;
    outStruct.PackageCMDLatch = ui8FALSE;
    outStruct.PackageCMDClose = ui8FALSE;
    outStruct.PackageCMDOpen = ui8FALSE;
    outStruct.TimeStamp = 0;
    return outStruct;
}

// Re-usable, portable, cross-platform setup() function
MODdeclareSETUP(Mn)
{
    MODDATAPTR_ERROR_RETURN(Mn);

    // Setup is running in the loop area to handle exceptions...
    IF_MODULE_ERROR(Mn)
    {
        CLEAR_MODULE_ERRORS(Mn);  // do nothing, clear flags        
        
    }
    // Setup is running in the setup area following power on
    else
    {
        linkAPIioDevices(MODdataPTR(Mn));
    }
    return RETURN_SUCCESS;
}
void tryreadMotorData(struct smartMotorStruct* smartMotorStructPtrIn);
void trywriteMotorData(struct smartMotorStruct* smartMotorStructPtrIn);
void tryReadPacketsInterface(MODdeclarePTRIN(Mn));
void tryWritePacketsInterface(MODdeclarePTRIN(Mn));
void tryReadConsoleMenu(MODdeclarePTRIN(Mn));
void tryWriteConsoleMenu(MODdeclarePTRIN(Mn));
// Re-usable, portable, cross-platform loop() function
MODdeclareLOOP(Mn)
{
    int i;
    MODDATAPTR_ERROR_RETURN(Mn);

    // read api devices  
    // this code runs on both the FW and the Linux side
    // on the FW side, this bit is set true at construction
    // on the Linux side, this bit is set false at construction
    // on the Linux side, the API modules run as exe_threads
    if (MODdataPTR(Mn)->execAPIsMainLoop == ui8TRUE)
    {
        MODdataPTR(Mn)->TimeStamp = getMillis();
        tryReadPacketsInterface(MODdataPTR(Mn));
        tryReadConsoleMenu(MODdataPTR(Mn));
    }
    // read smart motor devices
    if(MODdataPTR(Mn)->execDEVsMainLoop)
    {
        for(i=0; i<NUMMOTORS; i++)
            tryreadMotorData(&MODdataPTR(Mn)->SmartMotors[i]);
    }

    // process - Open trigger
    if(MODdataPTR(Mn)->TriggerOpen)
    {
        // set the negative move trigger on each smart motor object
        for (i = 0; i < NUMMOTORS; i++)
            MODdataPTR(Mn)->SmartMotors[i].TriggerNegativeMove = ui8TRUE;
        MODdataPTR(Mn)->TriggerOpen = ui8FALSE;

        // if running on linux side, package open cmd packet
        MODdataPTR(Mn)->PackageCMDOpen = ui8TRUE;
    }
    // process - Close trigger
    else if(MODdataPTR(Mn)->TriggerClose)
    {
        // keep track of smart motor configured status
        UI_8 hasUnconfiguredFinger = ui8FALSE;
        // loop all motors
        for (i = 0; i < NUMMOTORS; i++)
        {
            // if low stop is configured
            if ((MODdataPTR(Mn)->SmartMotors[i].RotorPositionLowStop != LIMITSINITVAL))
            {
                // if the high stop is NOT configured
                if ((MODdataPTR(Mn)->SmartMotors[i].RotorPositionHighStop == -LIMITSINITVAL))
                    hasUnconfiguredFinger = ui8TRUE;
            }
        }
        // loop all motors, again
        for (i = 0; i < NUMMOTORS; i++)
        {
            // if low stop is configured
            if ((MODdataPTR(Mn)->SmartMotors[i].RotorPositionLowStop != LIMITSINITVAL))
            {
                // if an unconfigured motor exist
                if (hasUnconfiguredFinger)
                {
                    // check if this motor is unconfigured
                    if ((MODdataPTR(Mn)->SmartMotors[i].RotorPositionHighStop == -LIMITSINITVAL))
                    {
                        // trigger motion only on unconfigured finger
                        MODdataPTR(Mn)->SmartMotors[i].TriggerPositiveMove = ui8TRUE;
                        break;
                    }
                }
                else
                {
                    // trigger motion on every finger
                    MODdataPTR(Mn)->SmartMotors[i].TriggerPositiveMove = ui8TRUE;
                }
            }
        }
        MODdataPTR(Mn)->TriggerClose = ui8FALSE;

        // if running on linux side, package close cmd packet
        MODdataPTR(Mn)->PackageCMDClose = ui8TRUE;
    }
    // process - Latch trigger
    else if(MODdataPTR(Mn)->TriggerLatch)
    {
        // set the latch triggers on every smart motor object
        for (i = 0; i < NUMMOTORS; i++)
        {
            MODdataPTR(Mn)->SmartMotors[i].LatchHighStopPos = ui8TRUE;
            MODdataPTR(Mn)->SmartMotors[i].LatchLowStopPos = ui8TRUE;
        }
        MODdataPTR(Mn)->TriggerLatch = ui8FALSE;

        // if running on linux side, package close cmd packet
        MODdataPTR(Mn)->PackageCMDLatch = ui8TRUE;
    }
    // process - Stop trigger
    else if(MODdataPTR(Mn)->TriggerStop)
    {
        // trigger stop on every smart motor object
        for (i = 0; i < NUMMOTORS; i++)
            ccGripperDataPtrIn->SmartMotors[i].TriggerStop = ui8TRUE;
        MODdataPTR(Mn)->TriggerStop = ui8FALSE;

        // if running on linux side, package stop cmd packet
        MODdataPTR(Mn)->PackageCMDStop = ui8TRUE;
    }



    // state machine
    switch (MODdataPTR(Mn)->GripperMode[iCURRENT])
    {
    case gripper_uninitialized:  break;
    case gripper_opened:  break;
    case gripper_closed:  break;
    case gripper_stalled:  break;
    case gripper_inmotion:  break;
    default:  break;
    }
    MODdataPTR(Mn)->GripperMode[iLAST] = MODdataPTR(Mn)->GripperMode[iCURRENT];

    // write smart motor devices
    if(MODdataPTR(Mn)->execDEVsMainLoop)
    {
        for (i = 0; i < NUMMOTORS; i++)
        {       
            smartMotorLoop(&MODdataPTR(Mn)->SmartMotors[i]);
            trywriteMotorData(&MODdataPTR(Mn)->SmartMotors[i]);           
        }
    }

    // write api devices
    // this code runs on both the FW and the Linux side
    // on the FW side, this bit is set true at construction
    // on the Linux side, this bit is set false at construction
    // on the Linux side, the API modules run as exe_threads
    if (MODdataPTR(Mn)->execAPIsMainLoop == ui8TRUE)
    {
        tryWritePacketsInterface(MODdataPTR(Mn));       

        // Auto Update for Console Menu
        if (MODdataPTR(Mn)->ConsoleMenu.devptr->triggerWriteOperation != ui8TRUE)
        {
            MODdataPTR(Mn)->AutoUpdateNow = MODdataPTR(Mn)->TimeStamp;
            if (MODdataPTR(Mn)->AutoUpdate == ui8TRUE)
            {
                if (MODdataPTR(Mn)->AutoUpdateNow - MODdataPTR(Mn)->AutoUptateThen > 1000)
                {
                    MODdataPTR(Mn)->ConsoleMenu.devptr->triggerWriteOperation = ui8TRUE;
                    MODdataPTR(Mn)->AutoUptateThen = MODdataPTR(Mn)->TimeStamp;
                }
                if (MODdataPTR(Mn)->TimeStamp - MODdataPTR(Mn)->AutoUpdateOnAt > 3600000)
                    MODdataPTR(Mn)->AutoUpdate = ui8FALSE;
            }
            else
                MODdataPTR(Mn)->AutoUpdateOnAt = MODdataPTR(Mn)->TimeStamp;
            
        }

        tryWriteConsoleMenu(MODdataPTR(Mn));
    }
    return RETURN_SUCCESS;
}

MODdeclareSYSTICK(Mn) { ; }  // do nothing in the systick area



MODdeclarePRINTm(Mn)
{
    MODDATAPTR_RETURN(Mn);
    switch ((enum currentMenuNode)uiStructPtrIn->currentMenuIndex)
    {
    case cM_MainMenu:
        printGripperFWConsoleAPI(MODdataPTR(Mn), uiStructPtrIn);
        break;
    case cM_devMOT0:
        printSmartMotorConsoleAPI(&(MODdataPTR(Mn)->SmartMotors[0]), uiStructPtrIn);
        break;
    case cM_devMOT1:
        printSmartMotorConsoleAPI(&(MODdataPTR(Mn)->SmartMotors[1]), uiStructPtrIn);
        break;
   case cM_devMOT2:
        printSmartMotorConsoleAPI(&(MODdataPTR(Mn)->SmartMotors[2]), uiStructPtrIn);
        break;
    case cM_devMOT3:
        printSmartMotorConsoleAPI(&(MODdataPTR(Mn)->SmartMotors[3]), uiStructPtrIn);
        break;
    }
}


MODdeclarePARSEi(Mn)
{
    MODDATAPTR_RETURN(Mn);
    parseConsoleKeyPadAPI(ccGripperDataPtrIn, uiStructPtrIn);
    if (uiStructPtrIn->currentUserLevel > uiLevel_Observer)
    {
        parseGripperFWConsoleAPI(ccGripperDataPtrIn, uiStructPtrIn);
        parseSmartMotorConsoleAPI(&ccGripperDataPtrIn->SmartMotors[0], uiStructPtrIn);
        parseSuctionExtendConsoleAPI(&ccGripperDataPtrIn->VacuumExtend, uiStructPtrIn);
    }
    parseUserLevel(uiStructPtrIn);
    if (stringMatchCaseSensitive(&uiStructPtrIn->devptr->inbuff.charbuff[uiStructPtrIn->parseIndex], "Help") == ui8TRUE)
        uiStructPtrIn->showHelp = ui8TRUE;
    if (stringMatchCaseSensitive(&uiStructPtrIn->devptr->inbuff.charbuff[uiStructPtrIn->parseIndex], "Stop") == ui8TRUE)
    {
        int i;
        ccGripperDataPtrIn->OperationsLog[ccGripperDataPtrIn->OpsLogIndex++] = 3;
        if (ccGripperDataPtrIn->OpsLogSize < OPSLOGSIZE)
            ccGripperDataPtrIn->OpsLogSize++;
        if (ccGripperDataPtrIn->OpsLogIndex >= OPSLOGSIZE)
            ccGripperDataPtrIn->OpsLogIndex = 0;        

        MODdataPTR(Mn)->TriggerStop = ui8TRUE;
    }
}

#define IODEVPTR (MODdataPTR(Mn)->PacketsAPI.devptr)
#define PACKAPI (MODdataPTR(Mn)->PacketsAPI)
void tryReadPacketsInterface(MODdeclarePTRIN(Mn))
{
    static unsigned int cycleCounter = 0;


    //On the FW side, always try to read
    if ((IODEVPTR != nullptr) && (cycleCounter++ % 20 == 0))
    {
        ReadPacketBytes(&PACKAPI);
        if (IODEVPTR->newDataReadIn)
        {
            parseGripperPacketsAPI((ccGripperStruct*)&MODdataPTR(Mn)->compMod, &PACKAPI);

            IODEVPTR->triggerWriteOperation = ui8TRUE;
            IODEVPTR->newDataReadIn = ui8FALSE;
        }
    }
}
void tryWritePacketsInterface(MODdeclarePTRIN(Mn))
{
    //On the FW side, write when supposed to
    if (PACKAPI.devptr->triggerWriteOperation)
    {
        // package the data to send
        packageGripperPacketsAPI((ccGripperStruct*)&MODdataPTR(Mn)->compMod, &PACKAPI);
        
        // send the data
        WritePacketBytes(&PACKAPI);

        // reset trigger
        PACKAPI.devptr->triggerWriteOperation = ui8FALSE;
    }
}
#undef IODEVPTR
#undef PACKAPI


#define IODEVPTR (MODdataPTR(Mn)->ConsoleMenu.devptr)
#define MENU (MODdataPTR(Mn)->ConsoleMenu)
void tryReadConsoleMenu(MODdeclarePTRIN(Mn))
{
    if (IODEVPTR != nullptr)
    {
        GetMenuChars(&MENU);
        if (IODEVPTR->newDataReadIn)
        {
            MODparseINPUT(Mn)(&MODdataPTR(Mn)->compMod, &MENU);

            IODEVPTR->triggerWriteOperation = ui8TRUE;
            IODEVPTR->newDataReadIn = ui8FALSE;
        }
    }
}
void tryWriteConsoleMenu(MODdeclarePTRIN(Mn))
{
    if (MENU.devptr->triggerWriteOperation)
    {
        MODdataPTR(Mn)->AutoUptateThen = MODdataPTR(Mn)->TimeStamp;
        MENU.lines2print = 1;
        MENU.linesprinted = 0;
        MENU.clearScreen = ui8TRUE;
        OPENDOWHILE((&(MENU)))
            MODprintMENU(Mn)(&MODdataPTR(Mn)->compMod, &MENU);
        CLOSEDOWHILE((&(MENU)))
        MENU.lines2print = 1;
        MENU.linesprinted = 0;
        OPENDOWHILE((&(MENU)))
            MODprintMENU(Mn)(&MODdataPTR(Mn)->compMod, &MENU);
        CLOSEDOWHILE((&(MENU)))
        MENU.lines2print = 1;
        MENU.linesprinted = 0;
        OPENDOWHILE((&(MENU)))
            MODprintMENU(Mn)(&MODdataPTR(Mn)->compMod, &MENU);
        CLOSEDOWHILE((&(MENU)))
        MENU.lines2print = 1;
        MENU.linesprinted = 0;
        OPENDOWHILE((&(MENU)))
            MODprintMENU(Mn)(&MODdataPTR(Mn)->compMod, &MENU);
        CLOSEDOWHILE((&(MENU)))
            MENU.lines2print = 1;
        MENU.linesprinted = 0;
        OPENDOWHILE((&(MENU)))
            MODprintMENU(Mn)(&MODdataPTR(Mn)->compMod, &MENU);
        CLOSEDOWHILE((&(MENU)))
        MENU.lines2print = 1;
        MENU.linesprinted = 0;
        OPENDOWHILE((&(MENU)))
        writeUIHelpString(&MENU);
        CLOSEDOWHILE((&(MENU)))
        MENU.showPrompt = ui8TRUE;
        MENU.devptr->outbuff.charbuff[0] = 0x00;
        WriteMenuLine(&MENU);

        MENU.devptr->triggerWriteOperation = ui8FALSE;
    }
}
#undef IODEVPTR
#undef MENU

// IQ Smart Motors only send/receive one parameter per message, following a read/process/write model
// its necessary to used a phased processing model for communications.
// a read must be preceeded by a write to request the data
#define MOTCOMMTIMEOUT (50) // ms to trigger comms recovery actions for the smart motors
void tryreadMotorData(struct smartMotorStruct* smartMotorStructPtrIn)
{
    UI_32 DeltaMS = 0;
    UI_32 NowMS = 0;
    switch (smartMotorStructPtrIn->MotorsCommPhase)
    {
    case 1:// position reply
    case 3:// velocity reply
    case 5:// current reply
    case 7:// temperature reply or PWMCmd reply
        // read 
        readMotorData(smartMotorStructPtrIn);
        // parse 
        if (smartMotorStructPtrIn->devptr->newDataReadIn == ui8TRUE)
        {
            smartMotorStructPtrIn->devptr->newDataReadIn = ui8FALSE;
            parseMotorData(smartMotorStructPtrIn);
            // advance comm state
            smartMotorStructPtrIn->MotorsCommPhase++;
        }
        else // protect with timeout
        {           
            NowMS = getMillis();
            if (NowMS >= smartMotorStructPtrIn->millisThen)
                DeltaMS = NowMS - smartMotorStructPtrIn->millisThen;
            else
                DeltaMS = NowMS + (0xffffffff - smartMotorStructPtrIn->millisThen);
            
            if (DeltaMS > MOTCOMMTIMEOUT)
            {
                smartMotorStructPtrIn->Connected = ui8FALSE;
                smartMotorStructPtrIn->MotorsCommPhase = 0;
            }
        }
        break;
    
    default: break;
    }
}
void trywriteMotorData(struct smartMotorStruct* smartMotorStructPtrIn)
{
    int i;
    switch (smartMotorStructPtrIn->MotorsCommPhase)
    {
    case 0:// request position  
        smartMotorStructPtrIn->millisNow = getMillis();
        if (smartMotorStructPtrIn->millisNow >= smartMotorStructPtrIn->millisThen)
            smartMotorStructPtrIn->millisThentoNow = smartMotorStructPtrIn->millisNow - smartMotorStructPtrIn->millisThen;
        else
            smartMotorStructPtrIn->millisThentoNow = smartMotorStructPtrIn->millisNow + (0xffffffff - smartMotorStructPtrIn->millisThen);
        smartMotorStructPtrIn->millisThen = smartMotorStructPtrIn->millisNow;



        if (smartMotorStructPtrIn->millisThentoNow > 0)
        {

#ifdef EVENODDMOTORS
            if (smartMotorStructPtrIn->Connected != ui8TRUE)
            {
                if (smartMotorStructPtrIn->ModuleID == 0)
                    smartMotorStructPtrIn->ModuleID = 1;
                else
                    smartMotorStructPtrIn->ModuleID = 0;
            }            
#endif
            if (smartMotorStructPtrIn->CommPeriodAbsoluteMinimum > smartMotorStructPtrIn->millisThentoNow)
            {
                smartMotorStructPtrIn->CommPeriodAbsoluteMinimum = smartMotorStructPtrIn->millisThentoNow;
            }
            if (smartMotorStructPtrIn->CommPeriodAbsoluteMaximum < smartMotorStructPtrIn->millisThentoNow)
            {
                smartMotorStructPtrIn->CommPeriodAbsoluteMaximum = smartMotorStructPtrIn->millisThentoNow;
            }
            smartMotorStructPtrIn->CommPeriodAvgs[smartMotorStructPtrIn->PeridoAvgSampleIndex++] = smartMotorStructPtrIn->millisThentoNow;
            if (smartMotorStructPtrIn->PeridoAvgSampleIndex >= PeriodAvgSamples)
                smartMotorStructPtrIn->PeridoAvgSampleIndex = 0;
            if (smartMotorStructPtrIn->PeriodAvgSampleSize < PeriodAvgSamples)
                smartMotorStructPtrIn->PeriodAvgSampleSize++;
            smartMotorStructPtrIn->CommPeriodAccumulation = 0;
            for (i = 0; i < smartMotorStructPtrIn->PeriodAvgSampleSize; i++)
            {
                smartMotorStructPtrIn->CommPeriodAccumulation += smartMotorStructPtrIn->CommPeriodAvgs[i];
            }
            if (smartMotorStructPtrIn->PeriodAvgSampleSize > 0)
                smartMotorStructPtrIn->CommPeriodAverage = smartMotorStructPtrIn->CommPeriodAccumulation / smartMotorStructPtrIn->PeriodAvgSampleSize;
            else
                smartMotorStructPtrIn->CommPeriodAverage = 0;
        }


    case 2:// request velocity
    case 4:// request current
    case 6:// request temperature or request pwm command
    case 8:// set current limit
    case 9:// set coast mode or set pwm command
        // package msg
        packageMotorData(smartMotorStructPtrIn);
        // send msg
        writeMotorData(smartMotorStructPtrIn);
        // advance comm state
        smartMotorStructPtrIn->devptr->numbytesReadIn = 0;
        if (smartMotorStructPtrIn->MotorsCommPhase == 9)
            smartMotorStructPtrIn->MotorsCommPhase = 0;
        else
            smartMotorStructPtrIn->MotorsCommPhase++;
        break;
    default: break;
    }
}
