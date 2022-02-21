/** \file Application_Solution.c
*   \brief Cross-Platform Portable ccNOos Tests Definitions

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

This source demonstrates the usage of the ccNOos library and its dependence
on other files in the library.  A platform specific main(.c,.cpp) file need
only instantiate, configure, then execute the execution system.

*/
#include "Application_Solution.h"



// Re-usable, portable, cross-platform (ccNOosTests create() function)
MODdeclareCREATE(Mn)(MODdeclareCREATEINS)
{
    MODdeclareSTRUCT(Mn) outStruct;
    outStruct.compMod = CreateComputeModuleStruct();

    outStruct.Terminal = createantennaStruct();
    outStruct.APT = createaptStruct();
    outStruct.TPM = createtpmStruct();
    outStruct.TxRx = createtxRxStruct();
    outStruct.WMM = createwmmStruct();

    int i;
    for (i = 0; i < charBuffMax; i++)
    {
        outStruct.LCDKeyPad.charbuff_In[i] = 0x00;
        outStruct.LCDKeyPad.charbuff_Out[i] = 0x00;
    }
    outStruct.LCDKeyPad.chars2Write = 0;
    outStruct.LCDKeyPad.charsRead = 0;

    return outStruct;
}
struct txRxStruct createtxRxStruct()
{
    struct txRxStruct outStruct;
    int i;
    for (i = 0; i < MAX_NUM_ATTENUATORS; i++)
    {
        outStruct.AttenuatorNeedsWriting[i] = ui8FALSE;
    }    
    outStruct.INDEX_Attenuator = 0;
    outStruct.CMD_AttenuatorBits = 0x00;
    return outStruct;
}
struct gpsStruct creategpsStruct()
{
    struct gpsStruct outStruct;
    outStruct.devptr = nullptr;
    outStruct.data = createGPSDataStruct();
    outStruct.Connected = ui8FALSE;
    outStruct.newGPSData = ui8FALSE;
    return outStruct;
}
struct eCompStruct createeCompStruct()
{
    struct eCompStruct outStruct;
    outStruct.devptr = nullptr;
    outStruct.Connected = ui8FALSE;
    outStruct.neweCompassData = ui8FALSE;
    outStruct.data = createEcompDataStruct();
    return outStruct;
}
struct aptStruct createaptStruct()
{
    struct aptStruct outStruct;
    outStruct.eCompass = createeCompStruct();
    outStruct.GPS = creategpsStruct();
    return outStruct;
}
struct freqConvStruct createfreqConvStruct()
{
    struct freqConvStruct outStruct;
    outStruct.data = CreateADRF6650Data();    
    outStruct.newFreqConvData = ui8FALSE;   
    return outStruct;
}
struct powerMeterStruct createPowerMeterStruct()
{
    struct powerMeterStruct outStruct;
    outStruct.data.BandwidthMHz = 0;
    outStruct.newPowerMeterValue = ui8FALSE;
    outStruct.data.PowerMeasuredinBanddB = 0;
    outStruct.data.current_value = 0;
    outStruct.data.value_good = ui8FALSE;
    return outStruct;
}
struct tpmStruct createtpmStruct()
{
    struct tpmStruct outStruct;
    outStruct.freqConverter = createfreqConvStruct();
    outStruct.powerMeter = createPowerMeterStruct();
    return outStruct;
}
struct wmmStruct createwmmStruct()
{
    struct wmmStruct outStruct;
    outStruct.magdeclination = 0;
    outStruct.lastGoodThreshold = 0;
    outStruct.lastGoodMultiplier = 0;
    outStruct.lastGoodmagdeclination = 0;
    outStruct.lastGoodlongitude = 0;
    outStruct.lastGoodlatitude = 0;
    outStruct.lastGoodaltitude = 0;
    return outStruct;
}
// Re-usable, portable, cross-platform (ccNOosTests setup() function)
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

    }
    return RETURN_SUCCESS;
}

// Re-usable, portable, cross-platform (ccNOosTests loop() function)
void ErrorInitState(MODdeclarePTRIN(Mn));
void NotAcquiredState(MODdeclarePTRIN(Mn));
void AcquiringState(MODdeclarePTRIN(Mn));
void AcquiredState(MODdeclarePTRIN(Mn));

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


MODdeclareLOOP(Mn)
{
    MODDATAPTR_ERROR_RETURN(Mn);

    // read devices    
    MODdataPTR(Mn)->parsed = MODparseINPUT(Mn)(compModPtrIn);
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
    MODdataPTR(Mn)->printed = MODprintMENU(Mn)(compModPtrIn);

    return RETURN_SUCCESS;
}

MODdeclareSYSTICK(Mn) { ; }  // do nothing in the systick area



MODdeclarePRINTm(Mn)
{
    MODDATAPTR_ERROR_RETURN(Mn);

    INIT_MENU_VARS(charBuffMax, MODdataPTR(Mn)->LCDKeyPad.charbuff_Out);
    // looping and printing
    // for looping...
    int lines2Print = 1;
    int linesPrinted = 0;
    if (MODdataPTR(Mn)->LCDKeyPad.charbuff_In[0] != 0x00)
    {
        MODdataPTR(Mn)->LCDKeyPad.charbuff_In[0] = 0x00;
        MODdataPTR(Mn)->LCDKeyPad.charsRead = 0;
        while (lines2Print > 0)
        {
            switch (linesPrinted)
            {
            case 0:
                PRINT_MENU_LN  "\033[2J\033[0;0H\n/////////// SatComACS Menu ///////////////////"    END_MENU_LN;
            case 1:
                PRINT_MENU_LN  "\n// Version %s %s //", ccNOosccNOos_VerString(), ccNOosccNOos_VerDateString()       END_MENU_LN;
            case 2:
                PRINT_MENU_LN  "\nLatitude: \t\t%8.4f",  MODdataPTR(Mn)->APT.GPS.data.lattitude  END_MENU_LN;
            case 3:
                PRINT_MENU_LN  "\nLongitude: \t\t%8.4f",  MODdataPTR(Mn)->APT.GPS.data.longitude  END_MENU_LN;
            case 4:
                PRINT_MENU_LN  "\nAltitude: \t\t%8.0f",  MODdataPTR(Mn)->APT.GPS.data.altitude  END_MENU_LN;
            case 5:
                PRINT_MENU_LN  "\nDate/Time (UTC): %2i/%2i/%4i %6.0f",  MODdataPTR(Mn)->APT.GPS.data.month, MODdataPTR(Mn)->APT.GPS.data.day, MODdataPTR(Mn)->APT.GPS.data.year, MODdataPTR(Mn)->APT.GPS.data.utctime  END_MENU_LN;
            case 6:
                PRINT_MENU_LN  "\nMagnetic Declination: \t%6.2f",  MODdataPTR(Mn)->WMM.lastGoodmagdeclination  END_MENU_LN;
            case 7:
                PRINT_MENU_LN  "\nAzimuth Heading: \t%6.2f",  MODdataPTR(Mn)->APT.eCompass.data.yaw  END_MENU_LN;
            case 8:
                PRINT_MENU_LN  "\nElevation Pitch: \t%6.2f",  MODdataPTR(Mn)->APT.eCompass.data.pitch  END_MENU_LN;
            case 9:
                PRINT_MENU_LN  "\nAPT Roll: \t\t%6.2f",  MODdataPTR(Mn)->APT.eCompass.data.roll  END_MENU_LN;

            case 10:
                PRINT_MENU_LN  "\n///////////////////////////////////////////////"      END_MENU_LN;
            case 11:
                PRINT_MENU_LN  "\nType \"satComACS:Var:Val;\" set Var to Val"      END_MENU_LN;
            case 12:
                PRINT_MENU_LN "\nInput>>" END_MENU_LN;
            default:
                lines2Print = 0;
                break;
            }

            MODdataPTR(Mn)->LCDKeyPad.chars2Write = charsWritten;
            linesPrinted++;

            if (lines2Print > 0)
                WriteMenuLine(&MODdataPTR(Mn)->LCDKeyPad.charbuff_Out[0]);
        }
    }
    return charsWritten;
}

MODdeclarePARSEi(Mn)
{
    MODDATAPTR_ERROR_RETURN(Mn);

    // loop characters from user input
    int i = 0;
    int j = 0, k = 0, l = 0;

    GetMenuChars(&MODdataPTR(Mn)->LCDKeyPad.charbuff_In[0]);
    if (MODdataPTR(Mn)->LCDKeyPad.charbuff_In[0] != 0x00)
    {
        MODdataPTR(Mn)->LCDKeyPad.charsRead++;
#define thisC MODdataPTR(Mn)->LCDKeyPad.charbuff_In[i]

        while ((thisC != 0x00) && (i < charBuffMax))
        {
            // find delimeter or terminator
            if (thisC == ASCII_colon)
            {
                if (j == 0)
                    j = i;
                else
                    k = i;
            }
            else if (thisC == ASCII_semicolon)
            {
                if (j == 0 || k == 0 || j >= k)
                    return 0u;

                // j index of first :
                // k index of second :
                // i index of terminator ;

                MODdataPTR(Mn)->LCDKeyPad.charbuff_In[j] = 0x00;
                if (stringMatchCaseSensitive(&MODdataPTR(Mn)->LCDKeyPad.charbuff_In[0], "satComACS"))
                {
                    MODdataPTR(Mn)->LCDKeyPad.charbuff_In[k] = 0x00;
                    if (stringMatchCaseSensitive(&MODdataPTR(Mn)->LCDKeyPad.charbuff_In[j + 1], "Tx"))
                    {
                        l = 0;
                    }
                    else if (stringMatchCaseSensitive(&MODdataPTR(Mn)->LCDKeyPad.charbuff_In[j + 1], "Rx"))
                    {
                        l = 1;
                    }
                    else if (stringMatchCaseSensitive(&MODdataPTR(Mn)->LCDKeyPad.charbuff_In[j + 1], "Xx"))
                    {
                        l = 2;
                    }
                    else
                        return 0u;

                    thisC = 0x00;
                    //                if (ATO_F(&MODdataPTR(Mn)->charbuff_In[k + 1], &MODdataPTR(Mn)->AttenuatorValues[l]))
                    //                {
                    //                    // limit command within range
                    //                    limitDATcmd(&MODdataPTR(Mn)->AttenuatorValues[l]);
                    //                    // set flag to update
                    //                    MODdataPTR(Mn)->AttenuatorNeedsWriting[l] = ui8TRUE;
                    //                }

                }
                else
                    return 0u;
            }
            i++;
        }
    }
#undef thisC
    return i;
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

void tryReadAPTData(struct aptStruct* aptStructPtrIn)
{
    // try read gps, if got string(s)
    if (readGPS(&aptStructPtrIn->GPS))
    {
        // try parse strings
        if (tryParseGPSData(&aptStructPtrIn->GPS.devptr->devdata.inbuff.charbuff[0], &aptStructPtrIn->GPS.data))
        {
            aptStructPtrIn->GPS.newGPSData = ui8TRUE; // this signals to state machine that new gps data is available from gps device
        }
        aptStructPtrIn->GPS.devptr->devdata.newDataReadIn = ui8FALSE;
    }
    // try read eCompass, if got string(s)
    if (readEcompass(&aptStructPtrIn->eCompass))
    {
        // try parse strings
        if (tryParseEcompData(&aptStructPtrIn->eCompass.devptr->devdata.inbuff.charbuff[0], &aptStructPtrIn->eCompass.data))
        {
            aptStructPtrIn->eCompass.neweCompassData = ui8TRUE; // this signals to state machine that new gps data is available from gps device
        }
        aptStructPtrIn->eCompass.devptr->devdata.newDataReadIn = ui8FALSE;
    }
}

void tryReadTPMData(struct tpmStruct* tpmStructPtrIn)
{
    readFreqConv(&tpmStructPtrIn->freqConverter);
    readPowerMeter(&tpmStructPtrIn->powerMeter);
}

void tryWriteTPMData(struct tpmStruct* tpmStructPtrIn)
{
    writeFreqConv(&tpmStructPtrIn->freqConverter);
    writePowerMeter(&tpmStructPtrIn->powerMeter);
}
