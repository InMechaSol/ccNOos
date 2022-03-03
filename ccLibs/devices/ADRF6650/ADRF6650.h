/** \file ADRF6650.h
*   \brief part of <a href="https://github.com/InMechaSol/ccNOos">ccNOos</a>, Declarations for straight C and C++ 

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

#ifndef ADRF6660_H
#define ADRF6660_H

#include "ADRF6650_Map.h"
#include "version_config.h"


struct ADRF6650DataStruct
{
    float DesiredCenterFreqMHz;
    float RequiredLOFreqMHz;
    UI_8 LockedOnRequiredLOFreq;
};

#define DVGA_MAX_ATTEN 43

typedef enum {
    DVGA_SPI_MODE,
    DVGA_UP_DOWN_MODE
}dvga_mode_t;

// public scoped functions
struct ADRF6650DataStruct CreateADRF6650Data(); // data initialization function - like a class constructor




#endif // ADRF6660_CLASS_H
