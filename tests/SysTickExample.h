/** \file SysTickExample.h
*   \brief Cross-Platform Portable SysTickExample Declarations

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
void SerializeTimeString(MODSTRUCTPTR_IN(MODULENAME));

// platform and application specific io device functions
void WriteMinLED(MODSTRUCTPTR_IN(MODULENAME));
void WriteSecLED(MODSTRUCTPTR_IN(MODULENAME));
void WriteTimeSerial(MODSTRUCTPTR_IN(MODULENAME));

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


#define __PLATFORM_APP_CLASS_SYSTICK(PLATNAME,MODNAME) class PLATFORM_APP_NAME(PLATNAME){\
    public:\
    linkedEntryPointClass setupListHead;\
    linkedEntryPointClass loopListHead;\
    linkedEntryPointClass systickListHead;\
    linkedEntryPointClass exceptionListHead;\
    MODCLASS_NAME(MODNAME) MODNAME##CompMod;\
    executionSystemClass* MODNAME##ExecutionSystemPtr;\
    PLATFORM_APP_NAME(PLATNAME)() :\
        MODNAME##CompMod(LIGHT_OFF),\
        setupListHead(& MODNAME##CompMod, nullptr),\
        loopListHead(& MODNAME##CompMod, nullptr),\
        systickListHead(nullptr, nullptr),\
        exceptionListHead(&MODNAME##CompMod, nullptr)\
    {\
        MODNAME##ExecutionSystemPtr = & PLATFORM_EXESYS_NAME(PLATNAME);\
        MODNAME##ExecutionSystemPtr->LinkTheListsHead(\
            &setupListHead,\
            &loopListHead,\
            &systickListHead,\
            &exceptionListHead\
        );\
    }\
}
#define PLATFORM_APP_CLASS_SYSTICK(PLATNAME,MODNAME) __PLATFORM_APP_CLASS_SYSTICK(PLATNAME,MODNAME)


#endif // !__cplusplus
#endif // !systick example
