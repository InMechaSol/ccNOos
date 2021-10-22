/** \file execution_system.c
    \brief ccNOos Module Execution System Definitions

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
#include "execution_system.h" // -> includes version_config.h
#include "../consoleMenu/console_menu.h" // -> includes api_comp_mod.h -> compute_module.h -> version_config.h
#include "../deviceCompModule/dev_comp_mod.h" // -> includes io_device.h -> version_config.h

struct executionSystemStruct CreateExecutionSystemStruct(
    struct linkedEntryPointStruct* setupListHeadIn,
    struct linkedEntryPointStruct* loopListHeadIn,
    struct linkedEntryPointStruct* sysTickListHeadIn,
    uint32_t uSperTick
    )
{
    struct executionSystemStruct outStruct;
    outStruct.setupListHead = setupListHeadIn;
    outStruct.loopListHead = loopListHeadIn;
    outStruct.sysTickListHead = sysTickListHeadIn;
    outStruct.uSecPerSysTick = uSperTick;
    return outStruct;
}

// Application Entry Points
int ExecuteMain(struct executionSystemStruct* exeStructIn)
{
    if(!exeStructIn)
        return RETURN_ERROR;
    
    // cross platform exe system setup
    exeStructIn->hourTicks = 0u;
    exeStructIn->uSecTicks = 0u;
    
    // platform exe system setup
    int retVal = platformSetup();
    
    // module setup execution area
    struct linkedEntryPointStruct* currentExeNode = exeStructIn->setupListHead;
    MODULE_EXE_AREA
    
    // platform exe system start
    retVal = platformStart();
    
    // module loop execution area
    currentExeNode = exeStructIn->loopListHead;
    for(;;)
    {        
        MODULE_EXE_AREA    
        platformLoopDelay();
    }
    return RETURN_SUCCESS;
}
int ExecuteSysTick(struct executionSystemStruct* exeStructIn)
{
    exeStructIn->uSecTicks += exeStructIn->uSecPerSysTick;
    if(exeStructIn->uSecTicks >= TIME_uS_PER_HR)
    {
        exeStructIn->uSecTicks = 0u;
        exeStructIn->hourTicks++;
    }
    
    // module systick execution area
    struct linkedEntryPointStruct* currentExeNode = exeStructIn->sysTickListHead;
    
    MODULE_EXE_AREA
    
    
    return RETURN_SUCCESS;
}

// Module API Functions
uint32_t getuSecTicks(struct executionSystemStruct* exeStructPtr)
{
    return exeStructPtr->uSecTicks;
}
uint32_t getHourTicks(struct executionSystemStruct* exeStructPtr)
{
    return exeStructPtr->hourTicks;
}