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


#ifdef Mn
#error ccNOos_Tests: Multiple Examples Selected for Compilation, Not Permitted
#else
#define Mn SysTickClock
#endif

#define TIME_STR_LEN (16u)
#define MIN_LED_INDEX (0u)
#define SEC_LED_INDEX (1u)
#define TIME_SERIAL_INDEX (2u)


MODdeclareSTRUCT(Mn)
{
    COMPMODFIRST;
    UI_32 secCount_Last, secCount, minCount_Last, minCount, hrCount;
    UI_8 MinLEDvalue, SecLEDvalue, Light_Off;
    char time[TIME_STR_LEN];
};

#define MODdeclareCREATEINS int lightOff
#define MODcallCREATEINS lightOff

MODdeclareCREATE(Mn)(MODdeclareCREATEINS);

// platform and application specific io device functions
void WriteMinLED(MODdeclarePTRIN(Mn));
void WriteSecLED(MODdeclarePTRIN(Mn));
void WriteTimeSerial(MODdeclarePTRIN(Mn));

// Re-usable, portable, cross-platform (SysTickClock example setup() function)
MODdeclareSETUP(Mn);
// Re-usable, portable, cross-platform (SysTickClock example  setup() function)
MODdeclareLOOP(Mn);
// Re-usable, portable, cross-platform (SysTickClock example  systick() function)
MODdeclareSYSTICK(Mn);

////////////////////////////////////////////////////////////////////////////////
// C SysTickClock Example Application - built from computeModuleClass and Execution System
#define __PLATFORM_APP_CTEMPLATE(MODNAME) \
    struct linkedEntryPointStruct setupListHead = {\
        nullptr,\
        (struct computeModuleStruct*)&MODdataINST(Mn),\
        MODsetup(Mn)\
        };\
    struct linkedEntryPointStruct loopListHead = {\
        nullptr,\
        (struct computeModuleStruct*)&MODdataINST(Mn),\
        MODloop(Mn)\
        };\
    struct executionEntryStruct exeEntryPoints = {\
        &setupListHead,\
        &loopListHead,\
        nullptr,\
        &setupListHead\
        };\
    void applicationConfig()\
    {\
        exeSystem = CreateExecutionSystemStruct(\
                uSEC_PER_CLOCK);\
        MODdataINST(Mn) = MODstructCREATE(Mn)(\
                LIGHT_OFF\
                );\
    }
#define  PLATFORM_APP_CTEMPLATE(MODNAME) __PLATFORM_APP_CTEMPLATE(MODNAME)

#ifdef __cplusplus
////////////////////////////////////////////////////////////////////////////////
// C++ SysTickClock Example Class - built from computeModuleClass
class MODCLASS_NAME(Mn) : public computeModuleClass {
private:
    MODdeclareDATA(Mn);
public:
    MODCLASS_NAME(Mn)(MODdeclareCREATEINS);
    MODCLASS_SETUP_INLINE(Mn);
    MODCLASS_LOOP_INLINE(Mn);
    MODCLASS_SYSTICK_INLINE(Mn);
    MODCLASS_ExcpHndlr_INLINE(Mn);
};

////////////////////////////////////////////////////////////////////////////////
// C++ SysTickClock Example Application - built from computeModuleClass and Execution System
#define __PLATFORM_APP_CLASS(MODNAME) class theApplicationClass{\
    public:\
    linkedEntryPointClass setupListHead;\
    linkedEntryPointClass loopListHead;\
    linkedEntryPointClass systickListHead;\
    linkedEntryPointClass exceptionListHead;\
    MODCLASS_NAME(MODNAME) MODNAME##CompMod;\
    executionSystemClass* theExecutionSystemPtr;\
    theApplicationClass() :\
        MODNAME##CompMod(LIGHT_OFF),\
        setupListHead(& MODNAME##CompMod, nullptr),\
        loopListHead(& MODNAME##CompMod, nullptr),\
        systickListHead(nullptr, nullptr),\
        exceptionListHead(&MODNAME##CompMod, nullptr)\
    {\
        theExecutionSystemPtr = &exeSystem;\
        theExecutionSystemPtr->LinkTheListsHead(\
            &setupListHead,\
            &loopListHead,\
            &systickListHead,\
            &exceptionListHead\
        );\
    }\
}
#define PLATFORM_APP_CLASS(MODNAME) __PLATFORM_APP_CLASS(MODNAME)


#endif // !__cplusplus

