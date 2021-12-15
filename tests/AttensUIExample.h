/** \file AttensUIExample.h
*   \brief Cross-Platform Portable AttensUIExample Declarations

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
// Attenuators UI Example
///////////////////////////////////////////////////////////////////////
#ifndef COMPILE_TESTS
#ifdef EXAMPLE_ATTEN_UI

#ifdef MODULENAME
#error ccNOos_Tests: Multiple Examples Selected for Compilation, Not Permitted
#else
#define MODULENAME AttenUI
#endif

#define CONSOLE_LINE_LEN (80u)
#define MAX_NUM_ATTENUATORS (3u)
#define MIN_ATTEN_VAL (0.0)
#define MAX_ATTEN_VAL (31.75)

MODSTRUCT(MODULENAME)
{
    COMPMODFIRST;
    float API_DATValue;
    uint8_t CMD_AttenuatorBits, INDEX_Attenuator, chars2Write, charsRead;
    float AttenuatorValues[MAX_NUM_ATTENUATORS];
    bool AttenuatorNeedsWriting[MAX_NUM_ATTENUATORS];
    char consoleLine[CONSOLE_LINE_LEN];
    char apiLine[CONSOLE_LINE_LEN];
};

#define MODSTRUCTCREATEINS 
#define MODSTRUCTCALLINS 

MODSTRUCT_CREATE_PROTO(MODULENAME);

// platform and application specific io device functions
void WriteAttenuators(MODSTRUCTPTR_IN(MODULENAME));
void ReadUserInput(MODSTRUCTPTR_IN(MODULENAME));
void WriteMenuLine(MODSTRUCTPTR_IN(MODULENAME));

// output factional part, write integral part to "intPartPtr"
float ModuloFloat(float floatValue, float* intPartPtr);

// Re-usable, portable, cross-platform (attenuator ui setup() function)
MODULE_FUNC_PROTO_SETUP(MODULENAME);

// Re-usable, portable, cross-platform (attenuator ui loop() function)
MODULE_FUNC_PROTO_LOOP(MODULENAME);

// Re-usable, portable, cross-platform (attenuator ui systick() function)
MODULE_FUNC_PROTO_SYSTICK(MODULENAME);

#ifdef __cplusplus
////////////////////////////////////////////////////////////////////////////////
// C++ Attenuator UI Class - built from computeModuleClass
MODULE_CLASS_DECLARE(MODULENAME);

#define __PLATFORM_APP_CLASS_ATTEN_UI(PLATNAME,MODNAME) class PLATFORM_APP_NAME(PLATNAME){\
    public:\
    linkedEntryPointClass setupListHead;\
    linkedEntryPointClass loopListHead;\
    linkedEntryPointClass systickListHead;\
    linkedEntryPointClass exceptionListHead;\
    MODCLASS_NAME(MODNAME) MODNAME##CompMod;\
    executionSystemClass* MODNAME##ExecutionSystemPtr;\
    PLATFORM_APP_NAME(PLATNAME)() :\
        MODNAME##CompMod(),\
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
#define PLATFORM_APP_CLASS_ATTEN_UI(PLATNAME,MODNAME) __PLATFORM_APP_CLASS_ATTEN_UI(PLATNAME,MODNAME)

#endif // !__cplusplus
#endif // !EXAMPLE_ATTEN_UI
#endif
