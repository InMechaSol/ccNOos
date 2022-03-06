/** \file HMC1119.h
*   \brief <a href="https://www.inmechasol.org/" target="_blank">IMS</a>:
		<a href="https://github.com/InMechaSol/ccNOos" target="_blank">ccNOos</a>,
		Declarations for straight C and C++ 

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

#ifndef HMC1119_H
#define HMC1119_H

#include "version_config.h"

#define MIN_ATTEN_VAL (0.0)
#define MAX_ATTEN_VAL (31.75)

struct DATStruct
{
    float DATCMD;
    UI_8 attenSetting;
};
struct DATStruct createDATStruct();
void limitDATcmd(struct DATStruct* DATptr);
// output factional part, write integral part to "intPartPtr"
float ModuloFloat(float floatValue, float* intPartPtr);
void CalcAttenuationBits(struct DATStruct* DATptr);
#define datcmdbit16(DATA)   ( (0b10000000 & DATA) >> 7 )
#define datcmdbit8(DATA)   ( (0b01000000 & DATA) >> 6 )
#define datcmdbit4(DATA)   ( (0b00100000 & DATA) >> 5 )
#define datcmdbit2(DATA)   ( (0b00010000 & DATA) >> 4 )
#define datcmdbit1(DATA)    ( (0b00001000 & DATA) >> 3 )
#define datcmdbit0_50(DATA) ( (0b00000100 & DATA) >> 2 )
#define datcmdbit0_25(DATA) ( (0b00000010 & DATA) >> 1 )


#endif // HMC1119_H
