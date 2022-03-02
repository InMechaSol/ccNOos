/** \file compute_module.h
*   \brief part of ccNOos, Declarations for straight C and C++ wrappers 

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

#ifndef __COMPUTE_MODULE__
#define __COMPUTE_MODULE__
    
#include "version_config.h"

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