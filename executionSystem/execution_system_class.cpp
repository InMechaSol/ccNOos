/** \file execution_system_class.cpp
    \brief ccNOos Module Execution System Class Wrappers

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

Be certain to compile only one compilation unit in cpp, 
 - not 1 in c and another in cpp 
 - all three files (.h .c and .cpp) 
 -- used to create single cpp compilation unit

The module execution system is aware of all compute modules and io devices
available to the controller application.  Its purpose is to execute compute
modules according to configurable execution policies and to link modules
with their corresponding platform io devices.  The execution system is
also an API to each of the modules and io devices, capable of providing platform
specific functions for things like system time, execution time, platform details, etc
    

*/
#include "execution_system.c" // -> includes all c stuff for cpp compilation

linkedIODeviceClass::linkedIODeviceClass(
            IODeviceClass* devPtrIn,
            linkedIODeviceClass* nextPtrIn
            )
{
    ;
}

linkedEntryPointClass::linkedEntryPointClass(  
                computeModuleClass* modulePtr,
                linkedEntryPointClass* nextPtr
                )
{
    ;
}

executionSystemClass::executionSystemClass(
    linkedEntryPointClass* setupListHeadIn,
    linkedEntryPointClass* loopListHeadIn,
    linkedEntryPointClass* sysTickListHeadIn,
    linkedEntryPointClass* exceptionListHeadIn,
    uint32_t uSperTick
    )
{
    data = CreateExecutionSystemStruct(
            uSperTick
            );
    
    setupListHead = setupListHeadIn;
    loopListHead = loopListHeadIn;
    sysTickListHead = sysTickListHeadIn;
    exceptionListHead = exceptionListHeadIn;
}

int executionSystemClass::ExecuteMain()
{
    return RETURN_SUCCESS;
}

void executionSystemClass::ExecuteSysTick()
{
    
}
