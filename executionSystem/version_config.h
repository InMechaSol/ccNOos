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
#define USING_STDINT 
#ifdef USING_STDINT
    #ifdef __cplusplus
        #include <cstdint>
    #else
        #include <stdint.h>
    #endif // !__cplusplus
#else
    typedef unsigned char uint8_t;
    typedef char int8_t;
    #if INTBYTES == 2
        #warning 2 byte int
        typedef unsigned int uint16_t;
        typedef int int16_t;
        typedef unsigned long int uint32_t;
        typedef long int int32_t;
    #elif INTBYTES == 4
        #warning 4 byte int
        typedef unsigned short int uint16_t;
        typedef short int int16_t;
        typedef unsigned int uint32_t;
        typedef int int32_t;
    #else
        #error unsupportted int size!!!
    #endif
#endif // !USING_STDINT

/////////////////////////////////////////////////////////////////////////////
// Compiler Configuration for nullptr support
//#define REDEFININGNULLPTR
#ifdef REDEFININGNULLPTR
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

#endif // !__VERSIONCONFIG__