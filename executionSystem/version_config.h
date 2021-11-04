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
#define COMPMODFIRST struct computeModuleStruct compMod

#define __MODSTRUCT(MODNAME) struct MODNAME##Struct
#define MODSTRUCT(MODNAME) __MODSTRUCT(MODNAME)

#define __MODSTRUCT_CREATE_PROTO(MODNAME) MODSTRUCT(MODNAME) MODNAME##_create( MODSTRUCTCREATEINS )
#define MODSTRUCT_CREATE_PROTO(MODNAME) __MODSTRUCT_CREATE_PROTO(MODNAME)

#define __MODSTRUCTPTR_IN(MODNAME) MODSTRUCT(MODNAME)* MODNAME##DataPtrIn
#define MODSTRUCTPTR_IN(MODNAME) __MODSTRUCTPTR_IN(MODNAME)

#define __MODSTRUCTDATA(MODNAME) MODSTRUCT(MODNAME) MODNAME##Data
#define MODSTRUCTDATA(MODNAME) __MODSTRUCTDATA(MODNAME)

#define __MODDATAPTR_ERROR_RETURN(MODNAME, DATAPTRNAME) \
        MODSTRUCT(MODNAME)  *DATAPTRNAME = (( MODSTRUCT( MODNAME ) *)(compModPtr));\
        if (DATAPTRNAME == nullptr)\
            return RETURN_ERROR;
#define MODDATAPTR_ERROR_RETURN(MODNAME, DATAPTRNAME) __MODDATAPTR_ERROR_RETURN(MODNAME, DATAPTRNAME)

#define __IF_MODULE_ERROR(DATAPTRNAME) if(DATAPTRNAME->compMod.exceptionFlags != 0u)
#define IF_MODULE_ERROR(DATAPTRNAME) __IF_MODULE_ERROR(DATAPTRNAME)

#define __CLEAR_MODULE_ERRORS(DATAPTRNAME) DATAPTRNAME->compMod.exceptionFlags = 0u
#define CLEAR_MODULE_ERRORS(DATAPTRNAME) __CLEAR_MODULE_ERRORS(DATAPTRNAME)

#define __MODULE_FUNC_PROTO_SETUP(MODNAME) int MODNAME##_setup(struct computeModuleStruct* compModPtr)
#define MODULE_FUNC_PROTO_SETUP(MODNAME) __MODULE_FUNC_PROTO_SETUP(MODNAME)

#define __MODULE_FUNC_PROTO_LOOP(MODNAME) int MODNAME##_loop(struct computeModuleStruct* compModPtr)
#define MODULE_FUNC_PROTO_LOOP(MODNAME) __MODULE_FUNC_PROTO_LOOP(MODNAME)

#define __MODULE_FUNC_PROTO_SYSTICK(MODNAME) void MODNAME##_systick(struct computeModuleStruct* compModPtr)
#define MODULE_FUNC_PROTO_SYSTICK(MODNAME) __MODULE_FUNC_PROTO_SYSTICK(MODNAME)



#ifdef __cplusplus

#define __MODCLASS_NAME(MODNAME) MODNAME##_class
#define MODCLASS_NAME(MODNAME) __MODCLASS_NAME(MODNAME)

#define __MODCLASS_CONSTRUCTOR_PROTO(MODNAME) MODCLASS_NAME(MODNAME)(MODSTRUCTCREATEINS)
#define MODCLASS_CONSTRUCTOR_PROTO(MODNAME) __MODCLASS_CONSTRUCTOR_PROTO(MODNAME)

#define __MODCLASS_SETUP_INLINE(MODNAME) int mod_setup(){return MODNAME##_setup((struct computeModuleStruct*)(&MODNAME##Data));}
#define MODCLASS_SETUP_INLINE(MODNAME) __MODCLASS_SETUP_INLINE(MODNAME)

#define __MODCLASS_LOOP_INLINE(MODNAME) int mod_loop(){return MODNAME##_loop((struct computeModuleStruct*)(&MODNAME##Data));}
#define MODCLASS_LOOP_INLINE(MODNAME) __MODCLASS_LOOP_INLINE(MODNAME)

#define __MODCLASS_SYSTICK_INLINE(MODNAME) void mod_systick(){return MODNAME##_systick((struct computeModuleStruct*)(&MODNAME##Data));}
#define MODCLASS_SYSTICK_INLINE(MODNAME) __MODCLASS_SYSTICK_INLINE(MODNAME)

#define __MODCLASS_ExcpHndlr_INLINE(MODNAME) int mod_excphandler(){return MODNAME##_setup((struct computeModuleStruct*)(&MODNAME##Data));}
#define MODCLASS_ExcpHndlr_INLINE(MODNAME) __MODCLASS_ExcpHndlr_INLINE(MODNAME)

#define __MODULE_CONSTRUCT_DEFINE(MODNAME) MODCLASS_NAME(MODNAME)::MODCLASS_CONSTRUCTOR_PROTO(MODNAME): computeModuleClass( & MODNAME##Data.compMod)
#define MODULE_CONSTRUCT_DEFINE(MODNAME) __MODULE_CONSTRUCT_DEFINE(MODNAME)

#define __MODULE_CLASS_DECLARE(MODNAME) class MODCLASS_NAME(MODNAME) : public computeModuleClass{\
private:\
    MODSTRUCTDATA(MODNAME);\
public:\
    MODCLASS_CONSTRUCTOR_PROTO(MODNAME);\
    MODCLASS_SETUP_INLINE(MODNAME);\
    MODCLASS_LOOP_INLINE(MODNAME);\
    MODCLASS_SYSTICK_INLINE(MODNAME);\
    MODCLASS_ExcpHndlr_INLINE(MODNAME);\
} 
#define MODULE_CLASS_DECLARE(MODNAME) __MODULE_CLASS_DECLARE(MODNAME)

#define __MODULE_CONSTRUCT_DATA_CREATE(MODNAME) MODNAME##Data = MODNAME##_create( MODSTRUCTCALLINS )
#define MODULE_CONSTRUCT_DATA_CREATE(MODNAME) __MODULE_CONSTRUCT_DATA_CREATE(MODNAME)

#endif // !__cplusplus
#endif // !__VERSIONCONFIG__