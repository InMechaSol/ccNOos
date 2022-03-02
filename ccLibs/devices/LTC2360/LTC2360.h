/** \file LTC2360.h
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

#ifndef LTC2360_H__
#define LTC2360_H__

#include "version_config.h"


struct LTC2360DataStruct
{
    float BandwidthMHz;
    float PowerMeasuredinBanddB;
    I_16 current_value;
    UI_8 value_good;
};


struct LTC2360DataStruct CreateLTC2360Data();



//void Setup(struct LTC2360DataStruct* LTC2360DataPtrIn);
//void InitSPI(struct LTC2360DataStruct* LTC2360DataPtrIn);
//UI_16 ReadValue(int num_of_averages, struct LTC2360DataStruct* LTC2360DataPtrIn);


#endif  // LTC2360_CLASS_H__
