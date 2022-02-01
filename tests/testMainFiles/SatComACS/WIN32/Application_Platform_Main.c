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
#include "..\..\..\testApps\SatComACS\PlatformApp_Serialization.h"

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
void writeAttenuatorValues(struct txRxStruct* txRxStructPtrIn) { ; }
void readGPS(struct gpsStruct* gpsStructPtrIn) { ; }
void readEcompass(struct eCompStruct* eCompStructPtrIn) { ; }
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
