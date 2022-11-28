/** \file packets_api.h
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
#include "version_config.h"

#ifndef __PACKETS_API__
#define __PACKETS_API__

#ifdef __USINGCONSOLEMENU
	#include "console_menu.h"
#else
	#include "compute_module.h"
	#include "io_device.h"
#endif // !__USINGCONSOLEMENU

// PacketsAPI Data Structure
struct packAPIStruct
{
    struct devicedatastruct* devptr;
    UI_8 inPackNum, outPackNum;
    UI_8 inPackID, outPackID;
    UI_8 inPackLen, outPackLen;
    UI_8 inPackType, outPackType;
    UI_8 Connected, resetStats;
    UI_32 inPackCounter, outPackCounter;
    UI_32 inPackBytesCounter, outPackBytesCounter;
};
struct packAPIStruct createpackAPIStruct();

void WritePacketBytes(struct packAPIStruct* packStructPtrin);
void ReadPacketBytes(struct packAPIStruct* packStructPtrin);

void parseHeader(struct packAPIStruct* packStructPtrIn);
void packageHeader(struct packAPIStruct* packStructPtrIn);

void packageToken(struct packAPIStruct* packStructPtrIn, void* TokenPtr, UI_8 TokenLen);
void packageTokenEndianSwapped(struct packAPIStruct* packStructPtrIn, void* TokenPtr, UI_8 TokenLen);

void parseToken(struct packAPIStruct* packStructPtrIn, void* TokenPtr, UI_8 TokenLen);
void parseTokenEndianSwapped(struct packAPIStruct* packStructPtrIn, void* TokenPtr, UI_8 TokenLen);

#ifdef __cplusplus

class packetsAPIClass // declaration of console menu class
{
protected:
    struct packAPIStruct* packDataPtr = nullptr;
public:
    packetsAPIClass(struct packAPIStruct* packDataPtrIn);
    struct packAPIStruct* getPacketdataPtr();
};

#endif // !__cplusplus

#endif // ! __PACKETS_API__
