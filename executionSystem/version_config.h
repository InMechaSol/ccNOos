/** \file version_config.h
*   \brief <a href="https://www.inmechasol.org/" target="_blank">IMS</a>:
		<a href="https://github.com/InMechaSol/ccNOos" target="_blank">ccNOos</a>,
		Declarations for straight C and C++ 

    Copyright 2021 <a href="https://www.inmechasol.org/" target="_blank">InMechaSol, Inc</a>    

    Licensed under the Apache License, Version 2.0(the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

Notes:
	(.c includes .h) - for straight C or
	(.cpp includes .c which includes .h) - for C++ wrapped straight C
	*Always compiled to a single compilation unit, either C or CPP, not both

*/

#ifndef __VERSIONCONFIG__
#define __VERSIONCONFIG__
 
#define ccNOos_MajorVer 0
#define ccNOos_MinorVer 0
#define ccNOos_BuildNumber 0

#define ccNOos_DevString dev
#define ccNOos_VerDate 08JAN2021

    
////////////////////////////////////////////////////////////////////////////////
// Compiler Static Assert Macro Stack 
//    - Useful to enforce correct config at compile time  
#define CTASTR2(pre,post) pre ## post
#define CTASTR(pre,post) CTASTR2(pre,post)
#define STATIC_ASSERT(cond,msg) \
    typedef struct { int CTASTR(static_assertion_failed_,msg) : !!(cond); } \
        CTASTR(static_assertion_failed_,__COUNTER__)    
    
//////////////////////////////////////////////////////////////////////////////
// Compiler Stringize Macros
//    - Convert macro name or macro value to string constant in code  
#define xstr(s) str(s)
#define str(s) #s
   
////////////////////////////////////////////////////////////////////////////
// Version Strings for Compile time and Run time
//
#ifdef ccNOos_ReleaseBuild
#define ccNOos_VersionNumber ccNOos_MajorVer.ccNOos_MinorVer.ccNOos_BuildNumber
#endif
#ifndef ccNOos_ReleaseBuild
#define ccNOos_VersionNumber ccNOos_MajorVer.ccNOos_MinorVer.ccNOos_BuildNumber-ccNOos_DevString
#endif

const char* ccNOosccNOos_VerString();
const char* ccNOosccNOos_VerDateString();

#define ccNOosVersionsTemplate \
    const char* ccNOosccNOos_VerString()\
    {\
        return xstr(ccNOos_VersionNumber);\
    }\
    const char* ccNOosccNOos_VerDateString()\
    {\
        return xstr(ccNOos_VerDate);\
    }

// an eco system wide restriction on maximum buffer size...
#ifndef charBuffMax
#define charBuffMax 128
#endif

/////////////////////////////////////////////////////////////////////////////
// COMPILER CONSTANTS

/** \def RETURN_SUCCESS
* \brief Function Return Value for Success
*/
#define RETURN_SUCCESS (0)

/** \def RETURN_ERROR
* \brief Function Return Value for ERROR
*/
#define RETURN_ERROR (-1)

/////////////////////////////////////////////////////////////////////////////
// Compiler Configuration for Platform Selection
#ifdef PLATFORM_PSoC4
    #define PLATFORM_NAME PSoC4
    #define MAIN_C_NOos_Wsystick
    #define INTSIZE32
#endif
#ifdef PLATFORM_QTCreatorC
    #define PLATFORM_NAME QTCreatorC
    #define MAIN_C_NOos_NOsystick
    #define INTSIZE32
#endif
#ifdef PLATFORM_WIN32
    #define PLATFORM_NAME Win32
    #define MAIN_CPP_NOos_NOsystick
    #define INTSIZE32
#endif
#ifdef PLATFORM_ARDUINO
    #define PLATFORM_NAME Arduino
    #define MAIN_CPP_NOos_NOsystick_Arduino
    #define INTSIZE16
#endif
#ifdef PLATFORM_ccOS
    #define PLATFORM_NAME ccOS
    #define INTSIZE_STD
#endif
#ifdef PLATFORM_CUSTOM
    #define PLATFORM_NAME Custom
    #define PLATFORM_MAIN_CUSTOM
#endif
#ifndef PLATFORM_NAME
    #error PLATFORM_NAME must be defined, see examples
#endif
    
////////////////////////////////////////////////////////////////////////////////
// Fixed Width Integer Data Types
//    - enforce correct config at compile time 
#ifdef INTSIZE_STD  // only used if compiling for ccOS layer
#include <stdint.h>
typedef uint8_t UI_8;
typedef uint16_t UI_16;
typedef uint32_t UI_32;
typedef uint64_t UI_64;
typedef int8_t I_8;
typedef int16_t I_16;
typedef int32_t I_32;
typedef int64_t I_64;
#define INTSIZE_FIXED
#define __USINGCONSOLEMENU
#define __USINGFLOATPRINTF
#endif
#ifdef INTSIZE8
typedef unsigned int UI_8;
typedef unsigned long UI_16;
typedef unsigned long long UI_32;
//typedef unsigned long long UI_64;
#define INTSIZE_FIXED
#endif
#ifdef INTSIZE16
typedef unsigned char UI_8;
typedef unsigned int UI_16;
typedef unsigned long UI_32;
typedef unsigned long long UI_64;
typedef char I_8;
typedef int I_16;
typedef long I_32;
typedef long long I_64;
#define INTSIZE_FIXED
#endif
#ifdef INTSIZE32
typedef unsigned char UI_8;
typedef unsigned short UI_16;
typedef unsigned int UI_32;
typedef unsigned long UI_64;  
typedef char I_8;
typedef short I_16;
typedef int I_32;
typedef long I_64; 
#define INTSIZE_FIXED
#endif
#ifndef INTSIZE_FIXED
    #error INTSIZEx must be defined as INTSIZE8, INTSIZE16, or INTSIZE32 bits in type
#endif 

////////////////////////////////////////////////////////////////////////////////
// Static Assert Calls 
//    - for Fixed Width Correctness
STATIC_ASSERT(sizeof(UI_8)==1, UI_8_must_be_8_bits);  
STATIC_ASSERT(sizeof(UI_16)==2, UI_16_must_be_16_bits); 
STATIC_ASSERT(sizeof(UI_32)==4, UI_32_must_be_32_bits); 
STATIC_ASSERT(sizeof(I_8)==1, I_8_must_be_8_bits);  
STATIC_ASSERT(sizeof(I_16)==2, I_16_must_be_16_bits); 
STATIC_ASSERT(sizeof(I_32)==4, I_32_must_be_32_bits);

///////////////////////////////////////////////////////////////////////////////
// True / False macros for UI_8 data type in boolean operatoins
#define ui8FALSE (0u)
#define ui8TRUE (1u)

/////////////////////////////////////////////////////////////////////////////
// Compiler Configuration for nullptr support
#ifdef REDEFINE_NULLPTR
    #warning redefining nullptr !!!
    #define nullptr NULL
#else
    #ifndef __cplusplus
        #define nullptr (0u)
    #endif
#endif

/////////////////////////////////////////////////////////////////////////////
// Compiler Configuration for Execution System Calls and Main Entry Points
#ifdef MAIN_C_NOos_Wsystick
    #define PLATFORM_MAIN
#endif
#ifdef MAIN_CPP_NOos_NOsystick
    #define PLATFORM_MAIN
#endif
#ifdef MAIN_CPP_NOos_NOsystick_Arduino
#define PLATFORM_MAIN
#endif
#ifdef MAIN_C_NOos_NOsystick
    #define PLATFORM_MAIN
#endif
#ifdef PLATFORM_ccOS
    #define PLATFORM_MAIN
#endif
#ifdef PLATFORM_MAIN_CUSTOM
    #define PLATFORM_MAIN
#endif
#ifndef PLATFORM_MAIN
    #error PLATFORM_MAIN must be defined, see examples
#endif

//////////////////////////////////////////////////////////////////////////////
// Template Macros to enforce Adherence to ccNOos concepts

// Naming Conventions Driven by Unique Module Name
//  formatted as an acceptable c language variable name

// Name of Module Structure Data Type
#define __MODstructTYPEname(mNAME) mNAME##Struct
#define MODstructTYPEname(mNAME) __MODstructTYPEname(mNAME)

// Name of Module Instance Data Variable
#define __MODdataINST(mNAME) mNAME##Data
#define MODdataINST(mNAME) __MODdataINST(mNAME)

// Name of Module Instance Data Pointer within Function Code
#define __MODdataPTR(mNAME) mNAME##DataPtrIn
#define MODdataPTR(mNAME) __MODdataPTR(mNAME)

// Name of Module Setup Function
#define __MODsetup(mNAME) setup_##mNAME
#define MODsetup(mNAME) __MODsetup(mNAME)

// Name of Module Loop Function
#define __MODloop(mNAME) loop_##mNAME
#define MODloop(mNAME) __MODloop(mNAME)

// Name of Module Systick Function
#define __MODsystick(mNAME) systick_##mNAME
#define MODsystick(mNAME) __MODsystick(mNAME)

// Name of Module Print Menu Function
#define __MODprintMENU(mNAME) printM_##mNAME
#define MODprintMENU(mNAME) __MODprintMENU(mNAME)

// Name of Module Parse Input Function
#define __MODparseINPUT(mNAME) parseI_##mNAME
#define MODparseINPUT(mNAME) __MODparseINPUT(mNAME)

// Name of Module Data Structure Initialization Function
#define __MODstructCREATE(mNAME) Create##mNAME##Struct
#define MODstructCREATE(mNAME) __MODstructCREATE(mNAME)


// Declaration Code Snippets using Naming Conventions
//  formatted as an acceptable c


// Declaration Code - Module Data Type - Structure Definition
#define __MODdeclareSTRUCT(mNAME) struct __MODstructTYPEname(mNAME)
#define MODdeclareSTRUCT(mNAME) __MODdeclareSTRUCT(mNAME)

// Declaration Code - Module Data Structure 1st element - must be compute module
#define COMPMODFIRST struct computeModuleStruct compMod

// Declare Inputs List - Module Functions operating on Module Data Pointer
#define __MODdeclarePTRIN(mNAME) struct __MODstructTYPEname(mNAME)* __MODdataPTR(mNAME)
#define MODdeclarePTRIN(mNAME) __MODdeclarePTRIN(mNAME)

// Declaration Code - Module Data Structure in C
#define __MODdeclareDATA(mNAME) struct __MODstructTYPEname(mNAME) __MODdataINST(mNAME)
#define MODdeclareDATA(mNAME) __MODdeclareDATA(mNAME)

// Declaration Code - Module Create Function Prototype
#define __MODdeclareCREATE(mNAME) __MODdeclareSTRUCT(mNAME) __MODstructCREATE(mNAME)
#define MODdeclareCREATE(mNAME) __MODdeclareCREATE(mNAME)

// Declaration Code - Module Setup Function Prototype
#define __MODdeclareSETUP(mNAME) int __MODsetup(mNAME) ( struct computeModuleStruct* compModPtrIn )
#define MODdeclareSETUP(mNAME) __MODdeclareSETUP(mNAME)

// Declaration Code - Module Loop Function Prototype
#define __MODdeclareLOOP(mNAME) int __MODloop(mNAME) ( struct computeModuleStruct* compModPtrIn )
#define MODdeclareLOOP(mNAME) __MODdeclareLOOP(mNAME)

// Declaration Code - Module Print Menu Function Prototype
#define __MODdeclarePRINTm(mNAME) void __MODprintMENU(mNAME) ( struct computeModuleStruct* compModPtrIn, struct uiStruct* uiStructPtrIn  )
#define MODdeclarePRINTm(mNAME) __MODdeclarePRINTm(mNAME)

// Declaration Code - Module Parse Menu Function Prototype
#define __MODdeclarePARSEi(mNAME) void __MODparseINPUT(mNAME) ( struct computeModuleStruct* compModPtrIn, struct uiStruct* uiStructPtrIn )
#define MODdeclarePARSEi(mNAME) __MODdeclarePARSEi(mNAME)

// Declaration Code - Module Systick Function Prototype
#define __MODdeclareSYSTICK(mNAME) void __MODsystick(mNAME) ( struct computeModuleStruct* compModPtrIn )
#define MODdeclareSYSTICK(mNAME) __MODdeclareSYSTICK(mNAME)

// Code Snippets to Simplify Common Tasks
//


// Snippet Code - Create and Cast Pointer to Module Data and Ensure not null
#define __MODDATAPTR_RETURN(mNAME) \
        __MODdeclarePTRIN(mNAME) = ( ( __MODdeclareSTRUCT(mNAME)* )(compModPtrIn));\
        if (__MODdataPTR(mNAME) == nullptr)\
            return;
#define MODDATAPTR_RETURN(mNAME) __MODDATAPTR_RETURN(mNAME)


// Snippet Code - Create and Cast Pointer to Module Data and Ensure not null
#define __MODDATAPTR_ERROR_RETURN(mNAME) \
        __MODdeclarePTRIN(mNAME) = ( ( __MODdeclareSTRUCT(mNAME)* )(compModPtrIn));\
        if (__MODdataPTR(mNAME) == nullptr)\
            return RETURN_ERROR;
#define MODDATAPTR_ERROR_RETURN(mNAME) __MODDATAPTR_ERROR_RETURN(mNAME)

// Snippet Code - Check for module errors
#define __IF_MODULE_ERROR(mNAME) if(__MODdataPTR(mNAME)->compMod.exceptionFlags != 0u)
#define IF_MODULE_ERROR( mNAME ) __IF_MODULE_ERROR( mNAME )

// Snippet Code - Clear module errors from within Check for module errors conditional block
#define __CLEAR_MODULE_ERRORS(mNAME) __MODdataPTR(mNAME)->compMod.exceptionFlags = 0u
#define CLEAR_MODULE_ERRORS(mNAME) __CLEAR_MODULE_ERRORS(mNAME)

// Main Function Template - C with an OS Linux or Windows or Similar non-real-time
#define __C_NOos_MAINnSYSTICK_TEMPLATE ccNOosVersionsTemplate \
int main()\
{\
    applicationConfig();\
    return ExecuteMain(&exeSystem, &exeEntryPoints);\
}\
void SysTickISRCallback(void);\
void SysTickISRCallback(void)\
{\
    ExecuteSysTick(&exeSystem, &exeEntryPoints);\
}
#define C_NOos_MAINnSYSTICK_TEMPLATE __C_NOos_MAINnSYSTICK_TEMPLATE

// Main Function Template - C with an OS Linux or Windows or Similar non-real-time
#define __C_OS_MAIN_TEMPLATE ccNOosVersionsTemplate \
int main(int argc, char** argv)\
{\
    clock_t tlast = clock();\
    clock_t tnow, tdelta;\
    UI_32* uSecTicksPtr = &exeSystem.uSecTicks;\
    UI_32* hourTicksPtr = &exeSystem.hourTicks;\
    applicationConfig();\
    ExecuteSetup(&exeSystem, &exeEntryPoints);\
    for (;;)\
    {\
        tnow = clock();\
        if (tnow >= tlast)\
            tdelta = tnow - tlast;\
        else\
            tdelta = tnow + (LONG_MAX - tlast);\
        tlast = tnow;\
        (*uSecTicksPtr) += tdelta * uSEC_PER_CLOCK;\
        if ((*uSecTicksPtr) >= TIME_uS_PER_HR)\
        {\
            (*uSecTicksPtr) = 0u;\
            (*hourTicksPtr)++;\
        }\
        ExecuteLoop(&exeSystem, &exeEntryPoints);\
    }\
    return RETURN_ERROR;\
}
#define C_OS_MAIN_TEMPLATE __C_OS_MAIN_TEMPLATE

//
// C++ specific macro code templates
#ifdef __cplusplus

// Name of Platform Specific Application
#define __PLATFORM_APP_NAME(PLATNAME) PLATNAME##_Application
#define PLATFORM_APP_NAME(PLATNAME) __PLATFORM_APP_NAME(PLATNAME)

// Name of Module Class Type
#define __MODCLASS_NAME(MODNAME) MODNAME##_class
#define MODCLASS_NAME(MODNAME) __MODCLASS_NAME(MODNAME)

// Declaration Code - Module Setup Inline wrapper
#define __MODCLASS_SETUP_INLINE(mNAME) int mod_setup(){return __MODsetup(mNAME)((struct computeModuleStruct*)(&__MODdataINST(mNAME)));}
#define MODCLASS_SETUP_INLINE(mNAME) __MODCLASS_SETUP_INLINE(mNAME)

// Declaration Code - Module Loop Inline wrapper
#define __MODCLASS_LOOP_INLINE(mNAME) int mod_loop(){return __MODloop(mNAME)((struct computeModuleStruct*)(&__MODdataINST(mNAME)));}
#define MODCLASS_LOOP_INLINE(mNAME) __MODCLASS_LOOP_INLINE(mNAME)

// Declaration Code - Module SysTick Inline wrapper
#define __MODCLASS_SYSTICK_INLINE(mNAME) void mod_systick(){return __MODsystick(mNAME)((struct computeModuleStruct*)(&__MODdataINST(mNAME)));}
#define MODCLASS_SYSTICK_INLINE(mNAME) __MODCLASS_SYSTICK_INLINE(mNAME)

// Declaration Code - Module Exception Handler Inline wrapper
#define __MODCLASS_ExcpHndlr_INLINE(mNAME) int mod_excphandler(){return __MODsetup(mNAME)((struct computeModuleStruct*)(&__MODdataINST(mNAME)));}
#define MODCLASS_ExcpHndlr_INLINE(mNAME) __MODCLASS_ExcpHndlr_INLINE(mNAME)

// Declaration Code - Module Setup Inline wrapper
#define __MODULE_CONSTRUCT_DEFINE(mNAME) MODCLASS_NAME(mNAME)::MODCLASS_CONSTRUCTOR_PROTO(mNAME): computeModuleClass( & __MODdataINST(mNAME).compMod)
#define MODULE_CONSTRUCT_DEFINE(mNAME) __MODULE_CONSTRUCT_DEFINE(mNAME)

// Declaration Code - Application Instance Declaration
#define __PLATFORM_APP_CPPTEMPLATE(PLATNAME) __PLATFORM_APP_NAME(PLATNAME) theApplicationExample;
#define PLATFORM_APP_CPPTEMPLATE(PLATNAME) __PLATFORM_APP_CPPTEMPLATE(PLATNAME)

// Main Function Template - CPP with an OS Linux or Windows or Similar non-real-time
#define __CPP_OS_MAIN_TEMPLATE ccNOosVersionsTemplate \
int main(int argc, char** argv)\
{\
    clock_t tlast = clock();\
    clock_t tnow, tdelta;\
    UI_32* uSecTicksPtr = &exeSystem.getExeDataPtr()->uSecTicks;\
    UI_32* hourTicksPtr = &exeSystem.getExeDataPtr()->hourTicks;\
    exeSystem.ExecuteSetup();\
    for (;;)\
    {\
        tnow = clock();\
        if (tnow >= tlast)\
            tdelta = tnow - tlast;\
        else\
            tdelta = tnow + (LONG_MAX - tlast);\
        tlast = tnow;\
        (*uSecTicksPtr) += tdelta * uSEC_PER_CLOCK;\
        if ((*uSecTicksPtr) >= TIME_uS_PER_HR)\
        {\
            (*uSecTicksPtr) = 0u;\
            (*hourTicksPtr)++;\
        }\
        exeSystem.ExecuteLoop();\
    }\
    return RETURN_ERROR;\
}
#define CPP_OS_MAIN_TEMPLATE __CPP_OS_MAIN_TEMPLATE

#define __CPP_MAIN_TEMPLATE_ARDUINO ccNOosVersionsTemplate \
unsigned long tlast;\
unsigned long tnow, tdelta;\
uint32_t* uSecTicksPtr;\
uint32_t* hourTicksPtr;\
void setup() {\
    tlast = millis();\
    tnow, tdelta;\
    uSecTicksPtr = &exeSystem.getExeDataPtr()->uSecTicks;\
    hourTicksPtr = &exeSystem.getExeDataPtr()->hourTicks;\
    exeSystem.ExecuteSetup();\
}\
\
void loop()\
{\
    tnow = millis();\
    if (tnow >= tlast)\
        tdelta = tnow - tlast;\
    else\
        tdelta = tnow + (0xffffffff - tlast);\
    tlast = tnow;\
\
    (*uSecTicksPtr) += tdelta * uSEC_PER_CLOCK;\
    if ((*uSecTicksPtr) >= TIME_uS_PER_HR)\
    {\
        (*uSecTicksPtr) = 0u;\
        (*hourTicksPtr)++;\
    }\
\
    exeSystem.ExecuteLoop();\
\
}
#define CPP_MAIN_TEMPLATE_ARDUINO __CPP_MAIN_TEMPLATE_ARDUINO

#endif // !__cplusplus
#endif // !__VERSIONCONFIG__
