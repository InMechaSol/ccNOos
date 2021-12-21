/** \file version_config.h
    \brief ccNOos Source Code Version and Configuration Declarations 

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

Intended as a common include for all ccNOos headers (packetbaseclass, iodevice_class, etc)	
As a matter of principle and strategy, each of the including headers is expected to
follow a coding methodology.  For example, Static C functions operating on a pointer to
a common data structure type are declared and intended for straight C compilation.  C++ Wrapper
classes are declared to wrap C functions and encapsulate instance data.  
At this lowest of cross-platform levels, stdlib dependency must be limited to datatypes, datatype sizes, and 
mathematical functions on those datatypes.  Typically these are in stdint.h, stdfloat.h, and stdmath.h.
stdint.h has wide support and is implemented on most modern embedded processors.  stdfloat.h and stdmath.h 
have less support but again, most modern embedded processors have implementations of stdfloat.h and stdmath.h.
    

*/
#ifndef __VERSIONCONFIG__
#define __VERSIONCONFIG__

////////////////////////////////////////////////////////////////////////////////
// Compiler Configuration for stdint support
#ifdef USING_STDINT
#ifdef __cplusplus
    #ifdef USING_CSTDINT
        #include <cstdint>
    #else
        extern "C" {
        #include <stdint.h>
                    }
    #endif
#else
    #include <stdint.h>    
    typedef unsigned char bool;
    #define false ((bool)0u)
    #define true (~false)
#endif // !__cplusplus
#else
    #ifdef USING_NONSTDINT_SHORTER
        #warning not using stdint/cstdint !!!
        typedef unsigned char uint8_t;
        typedef char int8_t;        
        #warning 2 byte int
        typedef unsigned int uint16_t;
        typedef int int16_t;
        typedef unsigned long int uint32_t;
        typedef long int int32_t;
    #endif
    #ifdef USING_NONSTDINT_LONGER   
        #warning not using stdint / cstdint !!!
        typedef unsigned char uint8_t;
        typedef char int8_t;
        #warning 4 byte int
        typedef unsigned short int uint16_t;
        typedef short int int16_t;
        typedef unsigned int uint32_t;
        typedef int int32_t;
    #endif
#endif // !USING_STDINT

/////////////////////////////////////////////////////////////////////////////
// Compiler Configuration for nullptr support
#ifdef REDEFINE_NULLPTR
    #waring redifining nullptr !!! 
    #define nullptr NULL
#else
    #ifndef __cplusplus
        #define nullptr (0u)
    #endif
#endif

/////////////////////////////////////////////////////////////////////////////
// MACROS for Compile Time knowledge of int sizes
#define INTBYTES sizeof(int)
#define SHORTBYTES sizeof(short int)
#define LONGBYTES sizeof(long int)
#define LONGLONGBYTES sizeof(long long int)

/** \def RETURN_SUCCESS
* \brief Function Return Value for Success
*/
#define RETURN_SUCCESS (0)

/** \def RETURN_ERROR
* \brief Function Return Value for ERROR
*/
#define RETURN_ERROR (-1)

//////////////////////////////////////////////////////////////////////////////
// Template Macros to enforce Adherence to ccNOos concepts

// Naming Conventions Driven by Unique Module Name
//  formatted as an acceptable c language variable name

// Name of Platform Specific executoin system Instance
#define __PLATFORM_EXESYS_NAME(PLATNAME) PLATNAME##exeSystem
#define PLATFORM_EXESYS_NAME(PLATNAME) __PLATFORM_EXESYS_NAME(PLATNAME)

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

// Name of Module Data Structure Initialization Function
#define __MODstructCREATE(mNAME) Create##mNAME##Struct
#define MODstructCREATE(mNAME) __MODstructCREATE(mNAME)


// Declaration Code Snippets using Naming Convention
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

// Declaration Code - Module Systick Function Prototype
#define __MODdeclareSYSTICK(mNAME) void __MODsystick(mNAME) ( struct computeModuleStruct* compModPtrIn )
#define MODdeclareSYSTICK(mNAME) __MODdeclareSYSTICK(mNAME)

// Declaration Code - Platform Specific executoin system Instance
#define __PLATFORM_EXESYS_DECLARE(PLATNAME) struct executionSystemStruct __PLATFORM_EXESYS_NAME(PLATNAME)
#define PLATFORM_EXESYS_DECLARE(PLATNAME) __PLATFORM_EXESYS_DECLARE(PLATNAME)

// Code Snippets to Simplify Common Tasks
//


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


// Main Function Template - CPP with an OS Linux or Windows or Similar non-real-time
#define __C_OS_MAIN_TEMPLATE(PLATNAME) int main(int argc, char** argv)\
{\
    clock_t tlast = clock();\
    clock_t tnow, tdelta;\
    uint32_t* uSecTicksPtr = &PLATFORM_EXESYS_NAME(PLATNAME).uSecTicks;\
    uint32_t* hourTicksPtr = &PLATFORM_EXESYS_NAME(PLATNAME).hourTicks;\
    applicationConfig();\
    ExecuteSetup(&PLATFORM_EXESYS_NAME(PLATFORM_NAME), &exeEntryPoints);\
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
        ExecuteLoop(&PLATFORM_EXESYS_NAME(PLATFORM_NAME), &exeEntryPoints);\
    }\
    return RETURN_ERROR;\
}
#define C_OS_MAIN_TEMPLATE(PLATNAME) __C_OS_MAIN_TEMPLATE(PLATNAME)


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

// Main Function Template - CPP with an OS Linux or Windows or Similar non-real-time
#define __CPP_OS_MAIN_TEMPLATE(PLATNAME) int main(int argc, char** argv)\
{\
    clock_t tlast = clock();\
    clock_t tnow, tdelta;\
    uint32_t* uSecTicksPtr = &PLATFORM_EXESYS_NAME(PLATNAME).getExeDataPtr()->uSecTicks;\
    uint32_t* hourTicksPtr = &PLATFORM_EXESYS_NAME(PLATNAME).getExeDataPtr()->hourTicks;\
    PLATFORM_EXESYS_NAME(PLATNAME).ExecuteSetup();\
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
        PLATFORM_EXESYS_NAME(PLATNAME).ExecuteLoop();\
    }\
    return RETURN_ERROR;\
}
#define CPP_OS_MAIN_TEMPLATE(PLATNAME) __CPP_OS_MAIN_TEMPLATE(PLATNAME)

#endif // !__cplusplus
#endif // !__VERSIONCONFIG__
