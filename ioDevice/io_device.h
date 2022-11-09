/** \file io_device.h
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

#ifndef __IO_DEVICE__
#define __IO_DEVICE__  
    
#include "version_config.h"


////////////////////////////////////////////////////////////////////////
/// Generic IO Device
////////////////////////////////////////////////////////////////////////

/** \enum devicestateenum
*	\brief enumerates states of the io device
*/
enum devicestateenum
{
	devstate_init = 0,
	devstate_opening = 1,
	devstate_open = 2,
	devstate_reading = 3,
	devstate_readdone = 4,
	devstate_writing = 5,
	devstate_writedone = 6
};
/** \union devicebufferunion
*	\brief char/byte ambiguous container for io data
*/
union devicebufferunion
{
    char charbuff[charBuffMax];
    unsigned char bytebuff[charBuffMax];
};
/** \struct devicedatastruct
*	\brief the common data struct of io devices
*/
struct devicedatastruct
{
	union devicebufferunion inbuff;
	union devicebufferunion outbuff;
	enum devicestateenum devstate;
	UI_8 newDataReadIn;
	UI_8 triggerWriteOperation;
    UI_16 numbytes2Read;
    UI_16 numbytesReadIn;
    UI_16 numbytes2Write;
    UI_16 numbytesWritten;
    UI_16 parseIndex;
    
};
struct devicedatastruct createDeviceStruct();

////////////////////////////////////////////////////////////////////////
/// SPI IO Device
////////////////////////////////////////////////////////////////////////
enum SPITypes
{
    SPIType_one,
    SPIType_two,
    SPIType_three,
    SPIType_four
};
struct SPIDeviceStruct
{
    enum SPITypes spitype;
    struct devicedatastruct devdata;
    UI_8 chipSelectEnabled;

};

////////////////////////////////////////////////////////////////////////
/// Serial UART IO Device
////////////////////////////////////////////////////////////////////////
enum portStopBitsEnum
{
    stop_none = 0,
    stop_one = 1,
    stop_two = 2,
    stop_onepointfive = 3
};
enum portParityEnum
{
    parity_none = 0,
    parity_even = 1,
    parity_odd = 2,
    parity_mark = 3,
    parity_space = 4
};
enum portHandshakeEnum
{
    handshake_none = 0,
    handshake_requesttosend = 1,
    handshake_requesttosendxonxoff = 2,
    handshake_xonxoff = 3
};

struct SerialDeviceStruct
{
    int baudRate;
    int dataBits;
    enum portStopBitsEnum stopBits;
    enum portParityEnum parity;
    enum portHandshakeEnum handshake;
    struct devicedatastruct devdata;
    int readIndex;
};


////////////////////////////////////////////////////////////////////////////////
// C only, struct of function pointers
// - not yet using this, may not use it, creates complications when switching from c to c++
#ifndef __cplusplus

struct ioDeviceStruct
{
    int (*open)();
    int (*close)();
    int (*read)();
    int (*write)();
};


////////////////////////////////////////////////////////////////////////////////
// C++, class of virtual methods
#else

class IODeviceClass
{	

protected:
	struct devicedatastruct* devdataptr = nullptr;
	virtual int opendevice() = 0;
	virtual int closedevice() = 0;
	virtual int readdevice() = 0;
	virtual int writedevice() = 0;
	virtual UI_8 isdeviceopen() = 0;

public:
	IODeviceClass(struct devicedatastruct* devdataptrin);
	int OpenDev();
	int CloseDev();
	int ReadDev();
	int WriteDev();
	bool IsDevOpen();
	void TriggerWriteOperation();
	bool NewDataReadIn();
	void ClearNewDataReadInFlag();
	bool ReadTriggerWriteOperationFlag();
	struct devicedatastruct* GetDevPtr();
};




// Abstract SPI IO Device
class SPI_DeviceClass : public IODeviceClass
{
protected:
	SPIDeviceStruct* spidevptr;
public:
	SPI_DeviceClass(struct SPIDeviceStruct* spidevdataptrin);
};



// Abstract Serial IO Device
class Serial_DeviceClass : public IODeviceClass
{
protected:
	SerialDeviceStruct* serialdevptr;
public:
	Serial_DeviceClass(struct SerialDeviceStruct* serialdevdataptrin);
};



#endif // !__cplusplus
#endif // !__IO_DEVICE__
