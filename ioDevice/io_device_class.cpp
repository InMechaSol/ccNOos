/** \file io_device_class.cpp
*   \brief part of <a href="https://github.com/InMechaSol/ccNOos">ccNOos</a>, Implementation for C++ wrappers 

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

struct devicedatastruct* IODeviceClass::GetDevPtr()
{
	return devdataptr;
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