/** \file ccNOos_tests.h
*   \brief Cross-Platform Portable ccNOos Tests Declarations

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

This header demonstrates the usage of the ccNOos library and its dependence
on other files in the library.  A platform specific main(.c,.cpp) file need
only include this single header to implement a platform specific ccNOos_tests
application.

*/
#ifndef __CCNOOS_TESTS__
#define __CCNOOS_TESTS__  
    
#include "../executionSystem/execution_system.h"    
#include "../consoleMenu/console_menu.h"   

///////////////////////////////////////////////////////////////////////
// SysTickClock Example
///////////////////////////////////////////////////////////////////////
#ifdef EXAMPLE_SYSTICK

#ifdef MODULENAME
    #error ccNOos_Tests: Multiple Examples Selected for Compilation, Not Permitted
#else
    #define MODULENAME SysTickClock
#endif

#define TIME_STR_LEN (16u)
#define MIN_LED_INDEX (0u)
#define SEC_LED_INDEX (1u)
#define TIME_SERIAL_INDEX (2u)

//struct SysTickClockStruct
MODSTRUCT(MODULENAME)
{
    COMPMODFIRST;
    uint32_t secCount_Last, secCount, minCount_Last, minCount, hrCount;
    uint8_t MinLEDvalue, SecLEDvalue, Light_Off;
    char time[TIME_STR_LEN];
};

#define MODSTRUCTCREATEINS int lightOff
#define MODSTRUCTCALLINS lightOff

MODSTRUCT_CREATE_PROTO(MODULENAME);

// platform and application specific time string serialization
void SerializeTimeString( MODSTRUCTPTR_IN(MODULENAME));

// platform and application specific io device functions
void WriteMinLED( MODSTRUCTPTR_IN(MODULENAME));
void WriteSecLED( MODSTRUCTPTR_IN(MODULENAME));
void WriteTimeSerial( MODSTRUCTPTR_IN(MODULENAME));

// Re-usable, portable, cross-platform (SysTickClock example setup() function)
MODULE_FUNC_PROTO_SETUP(MODULENAME);

// Re-usable, portable, cross-platform (SysTickClock example  setup() function)
MODULE_FUNC_PROTO_LOOP(MODULENAME);

// Re-usable, portable, cross-platform (SysTickClock example  systick() function)
MODULE_FUNC_PROTO_SYSTICK(MODULENAME);

#ifdef __cplusplus
////////////////////////////////////////////////////////////////////////////////
// C++ SysTickClock Example Class - built from computeModuleClass
MODULE_CLASS_DECLARE(MODULENAME);

#endif // !__cplusplus
#endif // !systick example

///////////////////////////////////////////////////////////////////////
// Attenuators UI Example
///////////////////////////////////////////////////////////////////////
#ifdef EXAMPLE_ATTEN_UI

#ifdef MODULENAME
    #error ccNOos_Tests: Multiple Examples Selected for Compilation, Not Permitted
#else
    #define MODULENAME AttenUI
#endif

#define CONSOLE_LINE_LEN (80u)
#define TIME_SERIAL_INDEX (2u)

MODSTRUCT(MODULENAME)
{
    COMPMODFIRST;
    char consoleLine[CONSOLE_LINE_LEN];
};

#define MODSTRUCTCREATEINS 
#define MODSTRUCTCALLINS 

MODSTRUCT_CREATE_PROTO(MODULENAME);

// platform and application specific menu line serialization
void SerializeMenuLine(MODSTRUCTPTR_IN(MODULENAME));
void ParseAPIString(MODSTRUCTPTR_IN(MODULENAME));

// platform and application specific io device functions
void WriteAttenuators(MODSTRUCTPTR_IN(MODULENAME));
void ReadUserInput(MODSTRUCTPTR_IN(MODULENAME));

// Re-usable, portable, cross-platform (attenuator ui setup() function)
MODULE_FUNC_PROTO_SETUP(MODULENAME);

// Re-usable, portable, cross-platform (attenuator ui setup() function)
MODULE_FUNC_PROTO_LOOP(MODULENAME);

// Re-usable, portable, cross-platform (attenuator ui systick() function)
MODULE_FUNC_PROTO_SYSTICK(MODULENAME);

#ifdef __cplusplus
////////////////////////////////////////////////////////////////////////////////
// C++ Attenuator UI Class - built from computeModuleClass
MODULE_CLASS_DECLARE(MODULENAME);

#endif // !__cplusplus
#endif // !EXAMPLE_ATTEN_UI

////////////////////////////////
// Compile Error if Examples not defining 
#ifndef MODULENAME
    #error MODULENAME must be defined - see examples
#endif
#ifndef MODSTRUCTCREATEINS
    #error MODSTRUCTCREATEINS must be defined - see examples
#endif
#ifndef MODSTRUCTCALLINS
    #error MODSTRUCTCALLINS must be defined - see examples
#endif

#endif // !__CCNOOS_TESTS__