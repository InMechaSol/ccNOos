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

// Include all things ccNOos_tests, which is an application of ccNOos
// ccNOos_tests is the application solution
#include "..\..\..\testApps\SysTick\PlatformApp_Serialization.h"


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
#if PLATFORM_NAME==QTCreatorC
#include "../../ccOS_Tests/ccOS/tests/testPlatforms/Platform_QTCreatorC.h"
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
// SysTick Example
///////////////////////////////////////////////////////////////////////
// 1) Minute LED Device Write
void WriteMinLED(MODdeclarePTRIN(Mn)) { ; }
// 2) Second LED Device Write
void WriteSecLED(MODdeclarePTRIN(Mn)) { ; }
// 3) Serial Device Write
void WriteTimeSerial(MODdeclarePTRIN(Mn))
{
    printf("%s", MODdataPTR(Mn)->time);
    fflush(stdout);
}









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
