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



// Re-usable, portable, cross-platform (SatComACS create() function)
MODdeclareCREATE(Mn)(MODdeclareCREATEINS)
{
    MODdeclareSTRUCT(Mn) outStruct;
    outStruct.compMod = CreateComputeModuleStruct();
    outStruct.Terminal = createantennaStruct();
    outStruct.APT = createaptStruct();
    outStruct.TPM = createtpmStruct();
    outStruct.TxRx = createtxRxStruct();
    outStruct.WMM = createwmmStruct();
    outStruct.ConsoleMenu = createuiStruct();
    outStruct.LCDKeyPad = createuiStruct();
    outStruct.execAPIsMainLoop = ui8TRUE;
    return outStruct;
}

// Re-usable, portable, cross-platform (SatComACS setup() function)
MODdeclareSETUP(Mn)
{
    MODDATAPTR_ERROR_RETURN(Mn);

    // Setup is running in the loop area to handle exceptions...
    IF_MODULE_ERROR(Mn)
    {
        CLEAR_MODULE_ERRORS(Mn);  // do nothing, clear flags        
        MODdataPTR(Mn)->Terminal.State = antState_Error;
    }
    // Setup is running in the setup area following power on
    else
    {
        linkAPIioDevices(MODdataPTR(Mn));
    }
    return RETURN_SUCCESS;
}

// Re-usable, portable, cross-platform (SatComACS loop() function)
void ErrorInitState(MODdeclarePTRIN(Mn));
void NotAcquiredState(MODdeclarePTRIN(Mn));
void AcquiringState(MODdeclarePTRIN(Mn));
void AcquiredState(MODdeclarePTRIN(Mn));
void UpdateTerminalState(MODdeclarePTRIN(Mn));
void tryReadLCDKeyPad(MODdeclarePTRIN(Mn));
void tryWriteLCDKeyPad(MODdeclarePTRIN(Mn));
void tryReadConsoleMenu(MODdeclarePTRIN(Mn));
void tryWriteConsoleMenu(MODdeclarePTRIN(Mn));
MODdeclareLOOP(Mn)
{
    MODDATAPTR_ERROR_RETURN(Mn);

    if (MODdataPTR(Mn)->execAPIsMainLoop == ui8TRUE)
    {
        // read api modules
        tryReadLCDKeyPad(MODdataPTR(Mn));
        tryReadConsoleMenu(MODdataPTR(Mn));
    }

    // read devices      
    tryReadAPTData(&MODdataPTR(Mn)->APT);
    tryReadTPMData(&MODdataPTR(Mn)->TPM);

    // process satcomACS
    UpdateTerminalState(MODdataPTR(Mn));
    switch (MODdataPTR(Mn)->Terminal.State)
    {
    case antState_NotAcquired: NotAcquiredState(MODdataPTR(Mn)); break;
    case antState_Acquiring: AcquiringState(MODdataPTR(Mn)); break;
    case antState_Acquired: AcquiredState(MODdataPTR(Mn)); break;
    case antState_Error: ErrorInitState(MODdataPTR(Mn)); break;
    default: ErrorInitState(MODdataPTR(Mn)); break;
    }

    // write devices
    tryWriteTPMData(&MODdataPTR(Mn)->TPM);
    writeAttenuatorValues(&MODdataPTR(Mn)->TxRx);
    writeModemSwitchValues(&MODdataPTR(Mn)->TxRx);
    
    if (MODdataPTR(Mn)->execAPIsMainLoop == ui8TRUE)
    {
        // write api modules
        tryWriteLCDKeyPad(MODdataPTR(Mn));
        tryWriteConsoleMenu(MODdataPTR(Mn));
    }

    return RETURN_SUCCESS;
}

MODdeclareSYSTICK(Mn) { ; }  // do nothing in the systick area


// Print Menu Screen , one line at a time...
MODdeclarePRINTm(Mn)
{
    MODDATAPTR_RETURN(Mn);
    switch ((enum currentMenuNode)uiStructPtrIn->currentMenuIndex)
    {
    case cM_RootNode:
        ;//writeRootNodeMenuScreen(&MODdataPTR(Mn), uiStructPtrIn);
        break;
    case cM_MainMenu:
        writeSatComACSMenuScreen(MODdataPTR(Mn), uiStructPtrIn);
        break;
    case cM_Devices:
        writeSatComACSDevicesMenuScreen(MODdataPTR(Mn), uiStructPtrIn);
        break;
    case cM_Terminal:
        writeTerminalMenuScreen(&MODdataPTR(Mn)->Terminal, uiStructPtrIn);
        break;
    case cM_ExecutionSystem:
        //writeExeSysMenuScreen(&MODdataPTR(Mn), uiStructPtrIn);
        break;
    case cM_devAPT:
        writeAPTMenuScreen(&MODdataPTR(Mn)->APT, uiStructPtrIn);
        break;
    case cM_devTXRX:
        writeTxRxMenuScreen(&MODdataPTR(Mn)->TxRx, uiStructPtrIn);
        break;
    case cM_devTPM:
        writeTPMMenuScreen(&MODdataPTR(Mn)->TPM, uiStructPtrIn);
        break;
    }
}

// parse from uistruct input buffer,
MODdeclarePARSEi(Mn) 
{
    MODDATAPTR_RETURN(Mn);

/*  switch ((enum currentMenuNode)uiStructPtrIn->currentMenuIndex)
    {
    case cM_MainMenu:   */
        parseSatComACSMenuAPI(MODdataPTR(Mn), uiStructPtrIn);
/*      break;
    case cM_Devices:
        parseSatComACSDevicesMenuAPI(MODdataPTR(Mn), uiStructPtrIn);
        break;
    case cM_Terminal:
        parseTerminalMenuAPI(&MODdataPTR(Mn)->Terminal, uiStructPtrIn);
        break;
    case cM_devAPT:
        parseAPTMenuAPI(&MODdataPTR(Mn)->APT, uiStructPtrIn);
        break;
    }    */    

}




void UpdateTerminalState(MODdeclarePTRIN(Mn))
{
    // check for new data from devices
    if (MODdataPTR(Mn)->APT.GPS.newGPSData)
    {
        // latch into terminal object
        MODdataPTR(Mn)->Terminal.GeoLocation.latitude = MODdataPTR(Mn)->APT.GPS.data.lattitude;
        MODdataPTR(Mn)->Terminal.GeoLocation.longitude = MODdataPTR(Mn)->APT.GPS.data.longitude;
        MODdataPTR(Mn)->Terminal.GeoLocation.altitude = MODdataPTR(Mn)->APT.GPS.data.altitude;
        // clear flag
        MODdataPTR(Mn)->APT.GPS.newGPSData = ui8FALSE;
        // set connected flag
        MODdataPTR(Mn)->APT.GPS.Connected = ui8TRUE;

    }
    // check for new data from eCompass
    if ((MODdataPTR(Mn)->APT.eCompass.neweCompassData))
    {
        // latch into terminal object
        MODdataPTR(Mn)->Terminal.ReflectorAttitude.roll = MODdataPTR(Mn)->APT.eCompass.data.roll;
        MODdataPTR(Mn)->Terminal.ReflectorAttitude.pitch = MODdataPTR(Mn)->APT.eCompass.data.pitch;
        MODdataPTR(Mn)->Terminal.ReflectorAttitude.yaw = MODdataPTR(Mn)->APT.eCompass.data.yaw;
        // clear flag
        MODdataPTR(Mn)->APT.eCompass.neweCompassData = ui8FALSE;
        // set connected flag
        MODdataPTR(Mn)->APT.GPS.Connected = ui8TRUE;
    }
    // check for new data from down converter
    if (MODdataPTR(Mn)->TPM.freqConverter.newFreqConvData)
    {
        if (MODdataPTR(Mn)->TPM.freqConverter.data.LockedOnRequiredLOFreq)
        {
            // latch into terminal object
        }
        // clear flag
        MODdataPTR(Mn)->TPM.freqConverter.newFreqConvData = ui8FALSE;
    }
    // check for new data from power meter
    if (MODdataPTR(Mn)->TPM.powerMeter.newPowerMeterValue)
    {
        if (MODdataPTR(Mn)->TPM.freqConverter.data.LockedOnRequiredLOFreq)
        {
            // latch into terminal object
            MODdataPTR(Mn)->Terminal.CommSystem.RxSignalStrengthMetric = MODdataPTR(Mn)->TPM.powerMeter.data.PowerMeasuredinBanddB;
        }
        // clear flag
        MODdataPTR(Mn)->TPM.powerMeter.newPowerMeterValue = ui8FALSE;
    }
}


void ErrorInitState(MODdeclarePTRIN(Mn))
{
    // if anything at all is wrong, for whatever reason
    //  it must be resolved cyclically here

    // only if everything is clear should we transition state

}
void NotAcquiredState(MODdeclarePTRIN(Mn))
{
    // nothing at all is wrong, completely ready to acquire
    //  wating for command to do so

    // only transition on error or command to acquire

}
void AcquiringState(MODdeclarePTRIN(Mn))
{
    // nothing at all is wrong, 
    //  acquisition is in progress

    // only transition on error or acquisition

}
void AcquiredState(MODdeclarePTRIN(Mn))
{
    // nothing at all is wrong,
    //  just monitoring and responding to commands

    // only transition on error or commands to reacquire

}

void tryReadLCDKeyPad(MODdeclarePTRIN(Mn)) 
{ 
#define IODEVPTR MODdataPTR(Mn)->LCDKeyPad.devptr
#define MENU MODdataPTR(Mn)->LCDKeyPad
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
#undef IODEVPTR
#undef MENU
}
void tryWriteLCDKeyPad(MODdeclarePTRIN(Mn)) 
{ 
#define MENU (MODdataPTR(Mn)->LCDKeyPad)

    if (MENU.devptr->triggerWriteOperation)
    {
        MENU.lines2print = 1;
        MENU.linesprinted = 0;
        MENU.clearScreen = ui8TRUE;
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
#undef MENU
}
void tryReadConsoleMenu(MODdeclarePTRIN(Mn))
{
#define IODEVPTR MODdataPTR(Mn)->ConsoleMenu.devptr
#define MENU MODdataPTR(Mn)->ConsoleMenu
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
#undef IODEVPTR
#undef MENU
}
void tryWriteConsoleMenu(MODdeclarePTRIN(Mn)) 
{ 
#define MENU (MODdataPTR(Mn)->ConsoleMenu)

    if (MENU.devptr->triggerWriteOperation)
    {
        MENU.lines2print = 1;
        MENU.linesprinted = 0;
        MENU.clearScreen = ui8TRUE;
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
#undef MENU
}