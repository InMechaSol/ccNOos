/** \file execution_system.h
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
    UI_32 uSecTicks;
    UI_32 hourTicks;
    UI_32 uSecPerSysTick;
};

// Execution System Data Structure Creation
struct executionSystemStruct CreateExecutionSystemStruct(
    UI_32 uSperTick
    );


////////////////////////////////////////////////////////////////////////////////
// Cross-Platform, Reusable, C/C++ Module API Functions
UI_32 getuSecTicks();
UI_32 getHourTicks();
UI_32 getuSecPerSysTick();

#ifdef __cplusplus
#define _DeclareExeSys executionSystemClass exeSystem(uSEC_PER_CLOCK);
#define _ExeSys_ exeSystem.getExeDataPtr()->
#else
#define _DeclareExeSys struct executionSystemStruct exeSystem;
#define _ExeSys_ exeSystem.
#endif

#define __ExeSysAPIFuncsTemplate \
_DeclareExeSys \
UI_32 getuSecTicks()\
{\
    return _ExeSys_ uSecTicks;\
}\
UI_32 getHourTicks()\
{\
    return _ExeSys_ hourTicks;\
}\
UI_32 getuSecPerSysTick()\
{\
    return _ExeSys_ uSecPerSysTick;\
}
#define ExeSysAPIFuncsTemplate __ExeSysAPIFuncsTemplate


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
            UI_32 ExcpIndex, 
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
int ExecuteSetup(
        struct executionSystemStruct* exeStructIn,
        struct executionEntryStruct* exeEntryPtrsIn
        );
int ExecuteLoop(
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
protected:
    struct executionSystemStruct data;
    linkedEntryPointClass* setupListHead = nullptr;
    linkedEntryPointClass* loopListHead = nullptr;
    linkedEntryPointClass* sysTickListHead = nullptr;
    linkedEntryPointClass* exceptionListHead = nullptr;
    void ModuleExeArea(int EXE_AREA_INDEX);
    void ModuleExceptionArea();
public:
    executionSystemClass(
                UI_32 uSperTick
                );
    virtual void ExecuteSetup();
    virtual void ExecuteLoop();
    int ExecuteMain();
    virtual void ExecuteSysTick();
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
