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
// SatCom Antenna Control System Example
///////////////////////////////////////////////////////////////////////
#ifndef COMPILE_TESTS
#ifdef EXAMPLE_SATCOM_ACS

#include "../ccLibs/acs/satComControl.h"
#include "../ccLibs/mcs/motionControl.h"

#ifdef Mn
#error ccNOos_Tests: Multiple Examples Selected for Compilation, Not Permitted
#else
#define Mn SatCom_ACS
#endif

MODdeclareSTRUCT(Mn)
{
    COMPMODFIRST;
};

#define MODdeclareCREATEINS 
#define MODcallCREATEINS 

MODdeclareCREATE(Mn)(MODdeclareCREATEINS);

// Re-usable, portable, cross-platform (ccNOosTests setup() function)
MODdeclareSETUP(Mn);
// Re-usable, portable, cross-platform (ccNOosTests loop() function)
MODdeclareLOOP(Mn);
// Re-usable, portable, cross-platform (ccNOosTests systick() function)
MODdeclareSYSTICK(Mn);
#ifdef __USINGCONSOLEMENU 
MODdeclarePRINTm(Mn);
MODdeclarePARSEi(Mn);
#endif

//////////////
// Sub - Modules
MODSTRUCT(ConsoleUISrv)
{
    COMPMODFIRST;

};
// Re-usable, portable, cross-platform (ConsoleUISrv setup() function)
MODULE_FUNC_PROTO_SETUP(ConsoleUISrv);
// Re-usable, portable, cross-platform (ConsoleUISrv loop() function)
MODULE_FUNC_PROTO_LOOP(ConsoleUISrv);
// Re-usable, portable, cross-platform (ConsoleUISrv systick() function)
MODULE_FUNC_PROTO_SYSTICK(ConsoleUISrv);


#ifdef __cplusplus
////////////////////////////////////////////////////////////////////////////////
// C++ SatCom ACS Example Class - built from computeModuleClass
MODULE_CLASS_DECLARE(MODULENAME);
MODULE_CLASS_DECLARE(ConsoleUISrv);

#define __PLATFORM_APP_CLASS_SATCOM_ACS(PLATNAME,MODNAME) class PLATFORM_APP_NAME(PLATNAME){\
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
#define PLATFORM_APP_CLASS_SATCOM_ACS(PLATNAME,MODNAME) __PLATFORM_APP_CLASS_SATCOM_ACS(PLATNAME,MODNAME)

#endif // !__cplusplus

#endif // !EXAMPLE_SATCOM_ACS

#endif  // !COMPILE_TESTS
