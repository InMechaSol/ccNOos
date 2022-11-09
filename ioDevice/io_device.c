/** \file io_device.c
*   \brief <a href="https://www.inmechasol.org/" target="_blank">IMS</a>:
		<a href="https://github.com/InMechaSol/ccNOos" target="_blank">ccNOos</a>,
		Implementation for straight C 

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

#include "io_device.h"
#include "execution_system.h"


struct devicedatastruct createDeviceStruct()
{
    struct devicedatastruct outstruct;
    outstruct.devstate = devstate_init;
    outstruct.newDataReadIn = ui8FALSE;
    outstruct.triggerWriteOperation = ui8FALSE;
    outstruct.numbytes2Read = 0;
    outstruct.numbytes2Write = 0;
    outstruct.numbytesReadIn = 0;
    outstruct.numbytesWritten = 0;
    outstruct.inbuff.charbuff[0] = 0x00;
    outstruct.outbuff.charbuff[0] = 0x00;
    outstruct.parseIndex = 0;
    return outstruct;
}

