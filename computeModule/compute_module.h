/** \file compute_module.h
    \brief ccNOos Compute Module Declarations 

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

A compute module is a fundamental code block, according to ccNOos 
philosophy, for Execution by the Execution System.  It provides a few
common entry points; all modules will have these common entry points
to be called by the execution system.

A compute module is a collection of entry points (functions) assignable
to unique execution levels.  The execution system is configured to "compute"
modules at particular execution levels.
    

*/    
#ifndef __COMPUTE_MODULE__
#define __COMPUTE_MODULE__
    
#include "../executionSystem/version_config.h"

struct computeModuleStruct
{
    UI_32 exceptionFlags;    
};

struct computeModuleStruct CreateComputeModuleStruct();

#ifdef __cplusplus
//class executionSystemClass; // forward declaration of execution system class

class computeModuleClass // declaration of compute module class
{
protected:
    struct computeModuleStruct *compModDataPtr = nullptr;
public:
    computeModuleClass(
            struct computeModuleStruct *compModDataPtrIn
            );
    virtual int mod_setup() = 0;
    virtual int mod_loop() = 0;
    virtual void mod_systick() = 0;
    virtual int mod_excphandler() = 0;
    struct computeModuleStruct* getModuleDataPtr();
};

#endif // !__cplusplus
#endif // ! __COMPUTE_MODULE__