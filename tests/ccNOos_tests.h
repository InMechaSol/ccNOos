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

// The COMPILE_TESTS switch selects between automated tests and example applications
#ifdef COMPILE_TESTS

#include "../executionSystem/execution_system.h"    
#include "../consoleMenu/console_menu.h"  

// Test declarations
// 1) Execution System Tests
//   a. Test SN_PrintF and ATO_ functions
//   b. Test Time API functions and Execution Time Accuracy
//   c. Test error handling and recovery
//   d. Test logging and configuration


#define Mn ccNOosTests

MODSTRUCT(Mn)
{
    COMPMODFIRST;
};

#define MODSTRUCTCREATEINS 
#define MODSTRUCTCALLINS 

MODSTRUCT_CREATE_PROTO(Mn);

// Re-usable, portable, cross-platform (attenuator ui setup() function)
MODULE_FUNC_PROTO_SETUP(Mn);

// Re-usable, portable, cross-platform (attenuator ui loop() function)
MODULE_FUNC_PROTO_LOOP(Mn);

// Re-usable, portable, cross-platform (attenuator ui systick() function)
MODULE_FUNC_PROTO_SYSTICK(Mn);

#ifdef __cplusplus
////////////////////////////////////////////////////////////////////////////////
// C++ Module Wrapper Class - built from computeModuleClass
MODULE_CLASS_DECLARE(Mn);

#define __PLATFORM_APP_CLASS_ccNOosTests(PLATNAME,MODNAME) class PLATFORM_APP_NAME(PLATNAME){\
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
#define PLATFORM_APP_CLASS_ccNOosTests(PLATNAME,MODNAME) __PLATFORM_APP_CLASS_ccNOosTests(PLATNAME,MODNAME)

#endif // !__cplusplus

#else // If not compiling for automated tests, then compile for example applications
///////////////////////////////////////////////////////////////////////
// SysTickClock Example
///////////////////////////////////////////////////////////////////////
#ifdef EXAMPLE_SYSTICK
#include "SysTickExample.h"
#endif // !systick example

///////////////////////////////////////////////////////////////////////
// Attenuators UI Example
///////////////////////////////////////////////////////////////////////
#ifdef EXAMPLE_ATTEN_UI
#include "AttensUIExample.h" 
#endif // !EXAMPLE_ATTEN_UI

///////////////////////////////////////////////////////////////////////
// SatCom ACS Example
///////////////////////////////////////////////////////////////////////
#ifdef EXAMPLE_SATCOM_ACS
#include "SatComACSExample.h" 
#endif // !EXAMPLE_SATCOM_ACS

////////////////////////////////
// Compile Error if Examples not defining 
#ifndef Mn
#error Mn must be defined - see examples
#endif
#ifndef MODdeclareCREATEINS
#error MODdeclareCREATEINS must be defined - see examples
#endif
#ifndef MODcallCREATEINS
#error MODcallCREATEINS must be defined - see examples
#endif
#endif // !COMPILE_TESTS
#endif // !__CCNOOS_TESTS__
