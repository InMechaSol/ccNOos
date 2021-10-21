/** \file execution_system.h
    \brief ccNOos Module Execution System Declarations 

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

The module execution system is aware of all compute modules and io devices
available to the controller application.  Its purpose is to execute compute
modules according to configurable execution policies and to link modules
with their corresponding platform io devices.  The execution system is
also an API to each of the modules and io devices, capable of providing platform
specific functions for things like system time, execution time, platform details, etc
    

*/
#ifndef __EXECUTION_SYSTEM__
#define __EXECUTION_SYSTEM__

#include "version_config.h"

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

#define TIME_us_PER_MIN (60000000u)
#define TIME_uS_PER_HR (3600000000u)
#define TIME_uS_PER_SEC (1000000u)

#define TIME_SEC_PER_MIN (60u)
#define TIME_MIN_PER_HR (60u) 

#define MODULE_EXE_AREA if(currentExeNode->entryPoint!=0u)\
    {   do\
        {\
            int retVal = currentExeNode->entryPoint(currentExeNode->dataPtr);\
            if(currentExeNode->nextPtr != 0u)\
            {\
                currentExeNode = currentExeNode->nextPtr;\
            }\
        }while(currentExeNode->nextPtr != 0u);\
    }

struct computeModuleStruct; // forward declaration of compute module data structure
struct ioDeviceStruct; // forward declaration of io device data structure

struct linkedEntryPointStruct
{
    struct linkedEntryPointStruct* nextPtr;
    void *dataPtr;
    int (*entryPoint)(void *dataPtr);
};

// Execution System Data Structure
struct executionSystemStruct
{
	struct linkedEntryPointStruct* setupListHead;
    struct linkedEntryPointStruct* loopListHead;
    struct linkedEntryPointStruct* sysTickListHead;
    uint32_t uSecTicks;
    uint32_t hourTicks;
    uint32_t uSecPerSysTick;
};

// Execution System Data Structure Creation and Linking
struct executionSystemStruct CreateExecutionSystemStruct(
    struct linkedEntryPointStruct* setupListHeadIn,
    struct linkedEntryPointStruct* loopListHeadIn,
    struct linkedEntryPointStruct* sysTickListHeadIn,
    uint32_t uSperTick
    );

// Platform Specific Configuration Functions
int platformSetup();
int platformStart();
// Application/Platform Configuration Function
int applicationConfig();


// Entry Points of the Execution System (module execution areas)
int ExecuteMain(struct executionSystemStruct* exeStructIn);
int ExecuteSysTick(struct executionSystemStruct* exeStructIn);


// Module API Functions
uint32_t getuSecTicks(struct executionSystemStruct* exeStructPtr);
uint32_t getHourTicks(struct executionSystemStruct* exeStructPtr);

#ifdef __cplusplus
}	// ! extern "C"

class computeModuleClass;  // forward declaration of compute module class

class executionSystemClass // declaration of execution system class
{

};


#endif // !__cplusplus
#endif // ! __EXECUTION_SYSTEM__