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

#include "../../../executionSystem/execution_system.h"
#include "../../../consoleMenu/console_menu.h"

#define Mn SatComACS

#define charBuffMax 80

MODdeclareSTRUCT(Mn)
{
    COMPMODFIRST;
    float   float_0, float_1;
    double  double_0, double_1;
    UI_64   ui64_0, ui64_1;
    I_64    i64_0, i64_1;
    UI_32   ui32_0, ui32_1;
    I_32    i32_0, i32_1;
    UI_16   ui16_0, ui16_1;
    I_16    i16_0, i16_1;
    UI_8    ui8_0, ui8_1;
    I_8     i8_0, i8_1;
    UI_8    charsRead, chars2Write;
    char    charbuff_In[charBuffMax];
    char    charbuff_Out[charBuffMax];
    UI_16   SerializationTestReturn;
    UI_16   TimedExecutionTestReturn;
    UI_16   ExceptionsTestReturn;
    UI_16   TestState;
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
///////////////////////////////////////////////////////////////////////
// Test Function Return Value Constants
#define RETURN_TEST_PASSED (0x0000)
#define RETURN_TEST_IN_PROGRESS (0x0001)
#define RETURN_FAILED_TIMEDEXECUTION (0x0010)
#define RETURN_FAILED_SERIALIZATION (0x0100)
#define RETURN_FAILED_DESERIALIZATION (0x0110)
#define RETURN_FAILED_COMPARISON (0x0120)

///////////////////////////////////////////////////////////////////////
// Test Functions
UI_16 TimedExecutionTest(MODdeclarePTRIN(Mn));
UI_16 ExceptionsTest(MODdeclarePTRIN(Mn));
#ifdef __USINGCONSOLEMENU
UI_16 SerializationTest(MODdeclarePTRIN(Mn));
const char* ResultsSerializationTests(MODdeclarePTRIN(Mn));
const char* ResultsTimedExecutionTests(MODdeclarePTRIN(Mn));
const char* StatusccNOosTests(MODdeclarePTRIN(Mn));
#endif

////////////////////////////////////////////////////////////////////////////////
// C ccNOosTests Example Application - built from computeModuleClass and Execution System
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
        exeSystem = CreateExecutionSystemStruct(uSEC_PER_CLOCK);\
        MODdataINST(Mn) = MODstructCREATE(Mn)();\
    }
#define  PLATFORM_APP_CTEMPLATE(MODNAME) __PLATFORM_APP_CTEMPLATE(MODNAME)

#ifdef __cplusplus
////////////////////////////////////////////////////////////////////////////////
// C++ ccNOosTests Example Class - built from computeModuleClass
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
// C++ ccNOosTests Example Application - built from computeModuleClass and Execution System
#define __PLATFORM_APP_CLASS(MODNAME) class theApplicationClass{\
    public:\
    linkedEntryPointClass setupListHead;\
    linkedEntryPointClass loopListHead;\
    linkedEntryPointClass systickListHead;\
    linkedEntryPointClass exceptionListHead;\
    MODCLASS_NAME(MODNAME) MODNAME##CompMod;\
    executionSystemClass* theExecutionSystemPtr;\
    theApplicationClass() :\
        MODNAME##CompMod(),\
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

#endif // !__CCNOOS_TESTS__
