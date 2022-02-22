/** \file io_device.c
*   \brief Cross-Platform Portable Input/Output Device Definitions

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

An io device provides a link to the real world from the compute solution.
It relies on serialized data of binary or text form.
Serialization, physical io device HW, and associated code are left to be
platform and application specific.  Device compute modules and api compute
modules designed to rely on this base io device concept, are then portable
to interface with any physical io device HW and utilize any serialization strategy.

*/
#include "io_device.h"
#include "execution_system.h"


struct devicedatastruct createDeviceStruct()
{
    struct devicedatastruct outstruct;
    outstruct.devstate = devstate_init;
    outstruct.newDataReadIn = ui8FALSE;
    outstruct.triggerWriteOperation = ui8FALSE;
    outstruct.inbuff.charbuff[0] = 0x00;
    outstruct.outbuff.charbuff[0] = 0x00;
    return outstruct;
}

