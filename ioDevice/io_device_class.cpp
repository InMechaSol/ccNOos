/** \file io_device_class.cpp
*   \brief Cross-Platform Portable Input/Output Device Class

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

An io device provides a link to the real world from the compute solution.
It relies on serialized data of binary or text form.
Serialization, physical io device HW, and associated code are left to be
platform and application specific.  Device compute modules and api compute
modules designed to rely on this base io device concept, are then portable
to interface with any physical io device HW and utilize any serialization strategy.

*/
#include "io_device.c"

void IODeviceClass::ClearNewDataReadInFlag()
{
	devdataptr->newDataReadIn = ui8FALSE;
}
bool IODeviceClass::ReadTriggerWriteOperationFlag()
{
	return devdataptr->triggerWriteOperation;
}
void IODeviceClass::TriggerWriteOperation()
{
	devdataptr->triggerWriteOperation = ui8TRUE;
}
bool IODeviceClass::NewDataReadIn()
{
	return devdataptr->newDataReadIn;
}

IODeviceClass::IODeviceClass(struct devicedatastruct* devdataptrin)
{
	devdataptr = devdataptrin;
}
int IODeviceClass::OpenDev()
{
	return opendevice();
}
int IODeviceClass::CloseDev()
{
	return closedevice();
}
int IODeviceClass::ReadDev()
{
	return readdevice();
}
int IODeviceClass::WriteDev()
{
	return writedevice();
}
bool IODeviceClass::IsDevOpen()
{
	return isdeviceopen();
}



SPI_DeviceClass::SPI_DeviceClass(struct SPIDeviceStruct* spidevdataptrin) :IODeviceClass(&spidevdataptrin->devdata) { spidevptr = spidevdataptrin; }

Serial_DeviceClass::Serial_DeviceClass(struct SerialDeviceStruct* serialdevdataptrin) : IODeviceClass(&serialdevdataptrin->devdata) { serialdevptr = serialdevdataptrin; }