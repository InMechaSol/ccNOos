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

#ifdef Mn
#error ccNOos_Tests: Multiple Examples Selected for Compilation, Not Permitted
#else
#define Mn AttenUI
#endif

#define CONSOLE_LINE_LEN (80u)
#define MAX_NUM_ATTENUATORS (3u)
#define MIN_ATTEN_VAL (0.0)
#define MAX_ATTEN_VAL (31.75)

MODdeclareSTRUCT(Mn)
{
    COMPMODFIRST;
    float API_DATValue;
    UI_8 CMD_AttenuatorBits, INDEX_Attenuator, chars2Write, charsRead;
    float AttenuatorValues[MAX_NUM_ATTENUATORS];
    UI_8 AttenuatorNeedsWriting[MAX_NUM_ATTENUATORS];
    char consoleLine[CONSOLE_LINE_LEN];
    char apiLine[CONSOLE_LINE_LEN];
};

#define MODdeclareCREATEINS
#define MODcallCREATEINS

MODdeclareCREATE(Mn)(MODdeclareCREATEINS);

// platform and application specific io device functions
void WriteAttenuators(MODdeclarePTRIN(Mn));
void ReadUserInput(MODdeclarePTRIN(Mn));
//void WriteMenuLine(MODdeclarePTRIN(Mn));

// output factional part, write integral part to "intPartPtr"
float ModuloFloat(float floatValue, float* intPartPtr);

// Re-usable, portable, cross-platform (AttenUI example setup() function)
MODdeclareSETUP(Mn);

// Re-usable, portable, cross-platform (AttenUI example  setup() function)
MODdeclareLOOP(Mn);

// Re-usable, portable, cross-platform (AttenUI example  systick() function)
MODdeclareSYSTICK(Mn);

////////////////////////////////////////////////////////////////////////////////
// C AttenUI Example Application - built from computeModuleClass and Execution System
#define __PLATFORM_APP_CTEMPLATE(PLATNAME,MODNAME) \
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
        PLATFORM_EXESYS_NAME(PLATFORM_NAME) = CreateExecutionSystemStruct(\
                uSEC_PER_CLOCK);\
        MODdataINST(Mn) = MODstructCREATE(Mn)( );\
    }
#define  PLATFORM_APP_CTEMPLATE(PLATNAME,MODNAME) __PLATFORM_APP_CTEMPLATE(PLATNAME,MODNAME)

#ifdef __cplusplus
////////////////////////////////////////////////////////////////////////////////
// C++ Attenuator UI Class - built from computeModuleClass
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

#define __PLATFORM_APP_CLASS(PLATNAME,MODNAME) class PLATFORM_APP_NAME(PLATNAME){\
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
#define PLATFORM_APP_CLASS(PLATNAME,MODNAME) __PLATFORM_APP_CLASS(PLATNAME,MODNAME)

#endif // !__cplusplus
#endif // !EXAMPLE_ATTEN_UI



#endif  // !COMPILE_TESTS
