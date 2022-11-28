/** \file Application_Serialization.h
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

#ifndef __GRIPPERSER__
#define __GRIPPERSER__

#include "ConsoleMenuAPI.h"
#include "PacketInterfaceAPI.h"



#define PREAMBLE (0x55)
#define ACCESS_GET (0)
#define ACCESS_SET (1)
#define ACCESS_SAVE (2)
#define ACCESS_REPLY (3)

#define TYPE_MULTITURN (59)
#define SUBTYPE_OBSANGDISP (11)
#define SUBTYPE_OBSANGVEL (12)
#define SUBTYPE_CTRLVEL (4)

#define TYPE_TEMPEST (77)
#define SUBTYPE_TEMP (0)

#define TYPE_BRUSHLESSDRV (50)
#define SUBTYPE_DRVMODE (0)
#define SUBTYPE_ILIMIT (34)
#define SUBTYPE_OBSANG (12)
#define SUBTYPE_OBSVEL (13)
#define SUBTYPE_ESTCUR (36)
#define SUBTYPE_DRVPWM (3)
#define SUBTYPE_DRVCOAST (6)

// Update a CRC accumulation with one data byte.
UI_16 ByteUpdateCrc(UI_16 crc, UI_8 data);

// Compute CRC word for a byte string.
UI_16 MakeCrc(const UI_8* data, UI_16 count);

#endif // !__GRIPPERSER__
