/** \file ccNOos_tests.c
*   \brief Cross-Platform Portable ccNOos Tests Definitions

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

This source demonstrates the usage of the ccNOos library and its dependence
on other files in the library.  A platform specific main(.c,.cpp) file need
only instantiate, configure, then execute the execution system.  

*/
#include "ccNOos_tests.h"

#ifdef COMPILE_TESTS

// Re-usable, portable, cross-platform (ccNOos_tests create() function)
MODSTRUCT_CREATE_PROTO(MODULENAME)
{
    MODSTRUCT(MODULENAME) outStruct;
    outStruct.compMod = CreateComputeModuleStruct();
    
    return outStruct;
}

// Re-usable, portable, cross-platform (ccNOos_tests setup() function)
MODULE_FUNC_PROTO_SETUP(MODULENAME)
{
    MODDATAPTR_ERROR_RETURN(MODULENAME, ccNOosTestsPtr);

    // Setup is running in the loop area to handle exceptions...
    IF_MODULE_ERROR(ccNOosTestsPtr)
    {
        CLEAR_MODULE_ERRORS(ccNOosTestsPtr);  // do nothing, clear flags
    }
    // Setup is running in the setup area following power on
    else
    {

    }
    return RETURN_SUCCESS;
}

// Re-usable, portable, cross-platform (ccNOos_tests loop() function)
MODULE_FUNC_PROTO_LOOP(MODULENAME)
{
    MODDATAPTR_ERROR_RETURN(MODULENAME, ccNOosTestsPtr);

   
    return RETURN_SUCCESS;
}

// Re-usable, portable, cross-platform (ccNOos_tests systick() function)
MODULE_FUNC_PROTO_SYSTICK(MODULENAME) { ; }  // do nothing in the systick area


#endif
