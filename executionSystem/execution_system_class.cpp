/** \file execution_system_class.cpp
*   \brief <a href="https://www.inmechasol.org/" target="_blank">IMS</a>:
		<a href="https://github.com/InMechaSol/ccNOos" target="_blank">ccNOos</a>,
		Implementation for C++ wrappers 

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

#include "execution_system.c" // -> includes all c stuff for cpp compilation

linkedIODeviceClass::linkedIODeviceClass(
            IODeviceClass* devPtrIn,
            linkedIODeviceClass* nextPtrIn
            )
{
    devPtr = devPtrIn;
    nextPtr = nextPtrIn;
}
IODeviceClass* linkedIODeviceClass::getDevPtr()
{
    return devPtr;
}
linkedIODeviceClass* linkedIODeviceClass::getNextIOClassPtr()
{
    return nextPtr;
}


linkedEntryPointClass::linkedEntryPointClass(  
                computeModuleClass* modulePtrIn,
                linkedEntryPointClass* nextPtrIn
                )
{
    modulePtr = modulePtrIn;
    nextPtr = nextPtrIn;
}
computeModuleClass* linkedEntryPointClass::getComputeModule()
{
    return modulePtr;
}
linkedEntryPointClass* linkedEntryPointClass::getNextEPClassPtr()
{
    return nextPtr;
}


executionSystemClass::executionSystemClass(
    UI_32 uSperTick
    )
{
    data = CreateExecutionSystemStruct(
            uSperTick
            );
}

void executionSystemClass::LinkTheListsHead(
    linkedEntryPointClass* setupListHeadIn,
    linkedEntryPointClass* loopListHeadIn,
    linkedEntryPointClass* sysTickListHeadIn,
    linkedEntryPointClass* exceptionListHeadIn
    )
{
    setupListHead = setupListHeadIn;
    loopListHead = loopListHeadIn;
    sysTickListHead = sysTickListHeadIn;
    exceptionListHead = exceptionListHeadIn;
}

// Variants of the "main" entry points 
void executionSystemClass::ExecuteSetup()
{
    // platform exe system setup
#ifndef __NOEXCEPTIONS
    try
    {
#endif
        platformSetup();
#ifndef __NOEXCEPTIONS
    }
    catch (...) {
        ;// WriteLogLine((char*)"Platform Setup Exception"); // exe sys logging
    }
#endif

    // module setup execution area
    ModuleExeArea(EXP_SETUP);

    // platform exe system start
#ifndef __NOEXCEPTIONS
    try
    {
#endif
        platformStart();
#ifndef __NOEXCEPTIONS
    }
    catch (...) {
        ;// WriteLogLine((char*)"Platform Start Exception"); // exe sys logging
    }
#endif
}
void executionSystemClass::ExecuteLoop()
{
    ModuleExceptionArea();

    ModuleExeArea(EXP_LOOP);

    platformLoopDelay();

}
int executionSystemClass::ExecuteMain()
{
    ExecuteSetup();    
    
    // module exception and loop execution areas
    for(;;)
    {
        ExecuteLoop();        
    }
    return RETURN_SUCCESS;
}

// The "systick" entry point
void executionSystemClass::ExecuteSysTick()
{
    data.uSecTicks += data.uSecPerSysTick;
    if(data.uSecTicks >= TIME_uS_PER_HR)
    {
        data.uSecTicks = 0u;
        data.hourTicks++;
    }
    
    // module systick execution area
    ModuleExeArea(EXP_SYSTICK);     
}

void executionSystemClass::ModuleExeArea(int EXE_AREA_INDEX)
{
    if(exceptionListHead!=nullptr)
    {
        linkedEntryPointClass* currentExeNode;
        switch(EXE_AREA_INDEX)
        {
            case EXP_SETUP: 
                currentExeNode = setupListHead;
                break;
            case EXP_LOOP: 
                currentExeNode = loopListHead;
                break;
            case EXP_SYSTICK: 
                currentExeNode = sysTickListHead;
                break;
            default:
                return;
        }
        
		if(currentExeNode != nullptr){
        if(currentExeNode->getComputeModule() != nullptr)
        {
            int retVal;
            do
            {

#ifndef __NOEXCEPTIONS
                try
                {
#endif
                    if(currentExeNode->getComputeModule()->getModuleDataPtr()->exceptionFlags == 0u)
                    {
                        switch(EXE_AREA_INDEX)
                        {
                            case EXP_SETUP: 
                                retVal = currentExeNode->getComputeModule()->mod_setup();
                                break;
                            case EXP_LOOP: 
                                retVal = currentExeNode->getComputeModule()->mod_loop();
                                break;
                            case EXP_SYSTICK: 
                                currentExeNode->getComputeModule()->mod_systick();
                                retVal = RETURN_SUCCESS;
                                break; 
                        }
                    }                        
                    else
                        retVal = RETURN_SUCCESS;

                    if(retVal != RETURN_SUCCESS)
#ifndef __NOEXCEPTIONS
                        throw RETURN_ERROR;
     
                }
                catch(...)
                {
#endif
                    currentExeNode->getComputeModule()->getModuleDataPtr()->exceptionFlags |= (0x00000001 << EXE_AREA_INDEX);      
#ifndef __NOEXCEPTIONS
                }
#endif                
                if(currentExeNode->getNextEPClassPtr() != nullptr)
                    currentExeNode = currentExeNode->getNextEPClassPtr();
                else 
                    break;
                
            }while(currentExeNode->getComputeModule() != nullptr);
        }}
    }
}
void executionSystemClass::ModuleExceptionArea()
{
    if(exceptionListHead!=nullptr)
    {
        linkedEntryPointClass* currentExeNode = exceptionListHead;
        if(currentExeNode!=nullptr)
        {
            int retVal;
            do
            {

#ifndef __NOEXCEPTIONS
                try
                {
#endif
                    if(currentExeNode->getComputeModule()->getModuleDataPtr()->exceptionFlags != 0u)
                        retVal = currentExeNode->getComputeModule()->mod_excphandler();
                    else
                        retVal = RETURN_SUCCESS;

                    if(retVal != RETURN_SUCCESS)
#ifndef __NOEXCEPTIONS
                        throw RETURN_ERROR;
                        
                }
                catch(...)
                {
#endif
                    currentExeNode->getComputeModule()->getModuleDataPtr()->exceptionFlags |= (0x00000001 << EXP_HANDLER);
#ifndef __NOEXCEPTIONS
                }
#endif
                
                if(currentExeNode->getNextEPClassPtr() != nullptr)
                    currentExeNode = currentExeNode->getNextEPClassPtr();
                else 
                    break;
                
            }while(currentExeNode->getComputeModule() != nullptr);
        }
    }    
}