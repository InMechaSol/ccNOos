/** \file dev_comp_mod.c
    \brief ccNOos Device Compute Module Definitions

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

A device compute module is a compute module with one or more io devices.
Its a compute module capable of interacting with external devices.  
Compute modules with general io device compliance can operate over any
platform specific io device.
    

*/
#include "dev_comp_mod.h"
#include "../executionSystem/execution_system.h"

struct deviceCompModStruct CreateDevCompModStruct(struct executionSystemStruct* exeSysPtrIn)
{
    struct deviceCompModStruct outStruct;
    outStruct.compMod = CreateComputeModuleStruct(exeSysPtrIn);
    
    return outStruct;
}