/** \file SatComACS.ino
*   \brief Arduino Implementation of SatComACS in c++

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

// Include all things ccNOos_tests, which is an application of ccNOos
// ccNOos_tests is the application solution
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
void writeAttenuatorValues(struct txRxStruct* txRxStructPtrIn) { ; }


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