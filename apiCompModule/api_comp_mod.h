/** \file api_comp_mod.h
    \brief ccNOos API Compute Module Declarations 

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

An API Module has access restrictions based on user access level.
An API Module has data access to the full execution system and all it contains.
An API Module has an IO device to serve and receive its serialized API data.
    

*/
#ifndef __API_COMP_MOD__
#define __API_COMP_MOD__

#include "../computeModule/compute_module.h"
#include "../ioDevice/io_device.h"

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus


struct apiCompModStruct // declaration of api compute module data structure
{
    
};

struct apiCompModStruct CreateAPICompModStruct();




#ifdef __cplusplus
}	// ! extern "C"


class apiCompModClass: public computeModuleClass // declaration of api compute module class
{

};


#endif // !__cplusplus
#endif // ! __API_COMP_MOD__