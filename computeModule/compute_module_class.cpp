/** \file compute_module_class.cpp
    \brief ccNOos Compute Module Class Wrappers

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

A compute module is a fundamental code block, according to ccNOos 
philosophy, for Execution by the Execution System.  It provides a few
common entry points; all modules will have these common entry points
to be called by the execution system.

A compute module is a collection of entry points (functions) assignable
to unique execution levels.  The execution system is configured to "compute"
modules at particular execution levels. 
    

*/


#include "compute_module.c"

computeModuleClass::computeModuleClass(
            struct computeModuleStruct *compModDataPtrIn,
            class executionSystemClass* exeSysPtrIn
            )
{
    
}
 