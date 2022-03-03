/** \file execution_system.c
*   \brief part of <a href="https://github.com/InMechaSol/ccNOos">ccNOos</a>, Implementation for straight C 

    Copyright 2021 InMechaSol, Inc

    https://www.inmechasol.org/

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

#include "execution_system.h"
#include "console_menu.h"


struct executionSystemStruct CreateExecutionSystemStruct(
    UI_32 uSperTick
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
            UI_32 ExcpIndex,
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

int ExecuteSetup(
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
    
    return RETURN_SUCCESS;
}

int ExecuteLoop(
        struct executionSystemStruct* exeStructIn,
        struct executionEntryStruct* exeEntryPtrsIn
        )
{
    // module exception and loop execution areas
    ModuleExceptionArea(
        exeEntryPtrsIn->exceptionListHead
        );

    ModuleExeArea(
        EXP_LOOP,
        exeEntryPtrsIn->loopListHead
        );

    platformLoopDelay();

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

