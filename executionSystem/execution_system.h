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

////////////////////////////////////////////////////////////////////////////////
// Time Constants
#define TIME_us_PER_MIN (60000000u)
#define TIME_uS_PER_HR (3600000000u)
#define TIME_uS_PER_SEC (1000000u)

#define TIME_SEC_PER_MIN (60u)
#define TIME_MIN_PER_HR (60u) 

////////////////////////////////////////////////////////////////////////////////
// Exception Flag Index
#define EXP_SETUP (0u)
#define EXP_LOOP (1u)
#define EXP_SYSTICK (2u)
#define EXP_PLATFORM (3u)
#define EXP_HANDLER (4u)

// Platform Specific Configuration Functions
void platformSetup();
void platformStart();
void platformLoopDelay();

////////////////////////////////////////////////////////////////////////////////
// Cross-Platform, Reusable, C/C++ Execution System Data Structure
struct executionSystemStruct
{	
    uint32_t uSecTicks;
    uint32_t hourTicks;
    uint32_t uSecPerSysTick;
};

// Execution System Data Structure Creation
struct executionSystemStruct CreateExecutionSystemStruct(
    uint32_t uSperTick
    );


////////////////////////////////////////////////////////////////////////////////
// Cross-Platform, Reusable, C/C++ Module API Functions
uint32_t getuSecTicks();
uint32_t getHourTicks();



////////////////////////////////////////////////////////////////////////////////
// C Execution System Base Components - not compiled in C++ build
#ifndef __cplusplus
   
struct computeModuleStruct;         // forward declaration
struct ioDeviceStruct;              // forward declaration

struct linkedIODeviceStruct
{
    struct ioDeviceStruct* devPtr;
    struct linkedIODeviceStruct* nextPtr;
};

struct linkedEntryPointStruct
{
    struct linkedEntryPointStruct* nextPtr;
    struct computeModuleStruct *dataPtr;
    int (*entryPoint)(
                    struct computeModuleStruct *dataPtrIn
                    );
};

struct executionEntryStruct
{
    struct linkedEntryPointStruct* setupListHead;
    struct linkedEntryPointStruct* loopListHead;
    struct linkedEntryPointStruct* sysTickListHead;
    struct linkedEntryPointStruct* exceptionListHead;
};

void ModuleExeArea(
            int ExcpIndex, 
            struct linkedEntryPointStruct* exeListHeadIn
            );

void ModuleExceptionArea(
            struct linkedEntryPointStruct* exeListHeadIn
            );

// Entry Points of the Execution System (module execution areas)
int ExecuteMain(
        struct executionSystemStruct* exeStructIn, 
        struct executionEntryStruct* exeEntryPtrsIn
        );
void ExecuteSysTick(
        struct executionSystemStruct* exeStructIn, 
        struct executionEntryStruct* exeEntryPtrsIn
        );

// Application/Platform Configuration Function
void applicationConfig();

#endif // !__cplusplus





#ifdef __cplusplus
////////////////////////////////////////////////////////////////////////////////
// Cross-Platform, Reusable, C++ Only Execution System Classes
class IODeviceClass; // forward declaration
class computeModuleClass; // forward declaration

class linkedIODeviceClass
{
private:
    IODeviceClass* devPtr = nullptr;
    linkedIODeviceClass* nextPtr = nullptr;
public:
    linkedIODeviceClass(
            IODeviceClass* devPtrIn,
            linkedIODeviceClass* nextPtrIn
            );
    IODeviceClass* getDevPtr();
    linkedIODeviceClass* getNextIOClassPtr();
};

class linkedEntryPointClass
{
private:
    computeModuleClass* modulePtr = nullptr;
    linkedEntryPointClass* nextPtr = nullptr;
public:
    linkedEntryPointClass(  
                computeModuleClass* modulePtrIn,
                linkedEntryPointClass* nextPtrIn
                );
    computeModuleClass* getComputeModule();
    linkedEntryPointClass* getNextEPClassPtr();
};

class executionSystemClass // declaration of execution system class
{
private:
    struct executionSystemStruct data;
    linkedEntryPointClass* setupListHead = nullptr;
    linkedEntryPointClass* loopListHead = nullptr;
    linkedEntryPointClass* sysTickListHead = nullptr;
    linkedEntryPointClass* exceptionListHead = nullptr;
    void ModuleExeArea(int EXE_AREA_INDEX);
    void ModuleExceptionArea();
public:
    executionSystemClass(
                uint32_t uSperTick
                );
    void ExecuteSetup();
    void ExecuteLoop();
    int ExecuteMain();
    void ExecuteSysTick();
    struct executionSystemStruct* getExeDataPtr() {return &data;}
    void LinkTheListsHead(
        linkedEntryPointClass* setupListHeadIn,
        linkedEntryPointClass* loopListHeadIn,
        linkedEntryPointClass* sysTickListHeadIn,
        linkedEntryPointClass* exceptionListHeadIn
        );
};


#endif // !__cplusplus
#endif // ! __EXECUTION_SYSTEM__