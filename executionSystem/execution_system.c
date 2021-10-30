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
#include "execution_system.h"
#include "../consoleMenu/console_menu.h"

struct executionSystemStruct CreateExecutionSystemStruct(
    uint32_t uSperTick
    )
{
    struct executionSystemStruct outStruct;
    outStruct.hourTicks = 0u;
    outStruct.uSecTicks = 0u;
    outStruct.uSecPerSysTick = uSperTick;
    return outStruct;
}



////////////////////////////////////////////////////////////////////////////////
// C Application Entry Points (not built in cpp build)
#ifndef __cplusplus

void ModuleExeArea(
            int ExcpIndex, 
            struct linkedEntryPointStruct* exeListHeadIn
            )
{
    struct linkedEntryPointStruct* currentExeNode = exeListHeadIn;
    int retVal;
    if(currentExeNode!=nullptr)
    {
        if(currentExeNode->entryPoint != nullptr)
        {   
            do
            {
                if(currentExeNode->dataPtr->exceptionFlags==0u)
                    retVal = currentExeNode->entryPoint(
                                        currentExeNode->dataPtr   
                                        );
                else
                    retVal = RETURN_SUCCESS;
                            
                if(retVal != RETURN_SUCCESS)
                    currentExeNode->dataPtr->exceptionFlags |= (0x00000001 << ExcpIndex);
                
                if(currentExeNode->nextPtr != nullptr)
                    currentExeNode = currentExeNode->nextPtr;
                else 
                    break;
                    
            }while(currentExeNode->entryPoint != nullptr);
        } 
    }    
}

void ModuleExceptionArea(
            struct linkedEntryPointStruct* exeListHeadIn
            )
{
    struct linkedEntryPointStruct* currentExeNode = exeListHeadIn;
    int retVal;
    if(currentExeNode!=nullptr)
    {
        if(currentExeNode->entryPoint != nullptr)
        {   
            do
            {
                if(currentExeNode->dataPtr->exceptionFlags!=0u)
                    retVal = currentExeNode->entryPoint(
                                        currentExeNode->dataPtr
                                        );
                else
                    retVal = RETURN_SUCCESS;
                            
                if(retVal != RETURN_SUCCESS)
                    currentExeNode->dataPtr->exceptionFlags |= (0x00000001 << EXP_HANDLER);
                
                if(currentExeNode->nextPtr != nullptr)
                    currentExeNode = currentExeNode->nextPtr;
                else 
                    break;
                    
            }while(currentExeNode->entryPoint != nullptr);
        } 
    }    
}
    
int ExecuteMain(
        struct executionSystemStruct* exeStructIn, 
        struct executionEntryStruct* exeEntryPtrsIn
        )
{
    if(exeStructIn == nullptr || exeEntryPtrsIn == nullptr)
        return RETURN_ERROR;
        
    // platform exe system setup
    platformSetup();
    
    // module setup execution area
    ModuleExeArea(
        EXP_SETUP, 
        exeEntryPtrsIn->setupListHead
        );
    
    // platform exe system start
    platformStart();
    
    // module exception and loop execution areas
    for(;;)
    {
        ModuleExceptionArea(
            exeEntryPtrsIn->exceptionListHead
            );
        
        ModuleExeArea(
            EXP_LOOP, 
            exeEntryPtrsIn->loopListHead
            );
        
        platformLoopDelay();
    }
    
    return RETURN_SUCCESS;
}

void ExecuteSysTick(
        struct executionSystemStruct* exeStructIn, 
        struct executionEntryStruct* exeEntryPtrsIn
        )
{
    exeStructIn->uSecTicks += exeStructIn->uSecPerSysTick;
    if(exeStructIn->uSecTicks >= TIME_uS_PER_HR)
    {
        exeStructIn->uSecTicks = 0u;
        exeStructIn->hourTicks++;
    }
    
    // module systick execution area
    ModuleExeArea(
            EXP_SYSTICK, 
            exeEntryPtrsIn->sysTickListHead
            );        
    
}
#endif

