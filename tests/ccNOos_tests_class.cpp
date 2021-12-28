/** \file ccNOos_tests_class.cpp
*   \brief Cross-Platform Portable ccNOos Class Wrappers

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

This source demonstrates the usage of the ccNOos library and its dependence
on other files in the library.  A platform specific main(.c,.cpp) file need
only instantiate, configure, then execute the execution system.  

*/
#include "ccNOos_tests.c"
;
#ifdef COMPILE_TESTS

MODCLASS_NAME(Mn)::MODCLASS_NAME(Mn)(MODdeclareCREATEINS) :
    computeModuleClass(&MODdataINST(Mn).compMod)
{
    MODdataINST(Mn) = MODstructCREATE(Mn)(MODcallCREATEINS);
}

#endif