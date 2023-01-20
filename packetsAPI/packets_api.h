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


// SPD packets strategy ???
// PackID - Selects SPD Array and Access Type
//          bits 0-1 : Access Type
//                 0 : Read
//                 1 : Write
//                 2 : ??
//                 3 : ??
//          bits 2-7 : SPD Array Selector (0-2^6-1)
// PackType - is the SPD index for the selected SPD Array


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

void parseSPD(struct packAPIStruct* packStructPtrin, struct SPDStruct* DataStructPtr);
void packageSPD(struct packAPIStruct* packStructPtrin, struct SPDStruct* DataStructPtr);

void parseSPDArray(struct packAPIStruct* packStructPtrin, struct SPDStruct* DataStructArray, UI_8 ArrayLen);
void packageSPDArray(struct packAPIStruct* packStructPtrin, struct SPDStruct* DataStructArray, UI_8 ArrayLen);


///////////////////////////////////////////////////////////////////////////////
// Serial Parameter Data Tags Type
#define NULL_TYPE -1
#define FLOAT_TYPE 0
#define SIGNED_TYPE 1
#define UNSIGNED_TYPE 2
struct SPDStruct
{
    int selcector;
    const char* labelString;
    const char* unitsString;
    unsigned char readonly;
    char type;
    unsigned int size;
    void* addr;
};
union SPD8
{
    unsigned char bytes[sizeof(UI_8)];
    char chars[sizeof(UI_8)];
    UI_8 uintVal;
    I_8 intVal;
};
union SPD16
{
    unsigned char bytes[sizeof(UI_16)];
    char chars[sizeof(UI_16)];
    UI_16 uintVal;
    I_16 intVal;
};
union SPD32
{
    unsigned char bytes[sizeof(UI_32)];
    char chars[sizeof(UI_32)];
    UI_32 uintVal;
    I_32 intVal;
    float floatVal;
};
union SPD64
{
    unsigned char bytes[sizeof(UI_64)];
    char chars[sizeof(UI_64)];
    UI_64 uintVal;
    I_64 intVal;
    double doubleVal;
};

// SPD Array Accessors
int getSPDSelector(int VarSelectionIn, struct SPDStruct* DataStructArray);
const char* getSPDLabelString(int VarSelectionIn, struct SPDStruct* DataStructArray);
const char* getSPDUnitsString(int VarSelectionIn, struct SPDStruct* DataStructArray);
UI_8 getSPDReadOnly(int VarSelectionIn, struct SPDStruct* DataStructArray);
char getSPDType(int VarSelectionIn, struct SPDStruct* DataStructArray);
unsigned int getSPDSize(int VarSelectionIn, struct SPDStruct* DataStructArray);
void* getSPDAddr(int VarSelectionIn, struct SPDStruct* DataStructArray);


float getSPDFloatValue(int VarSelectionIn, struct SPDStruct* DataStructArray);
void setSPDFloatValue(float inVal, int VarSelectionIn, struct SPDStruct* DataStructArray);



UI_8 setSPDFromString(char* inString, struct SPDStruct* DataStructPtr);
UI_8 packageSPDFromString(struct packAPIStruct* packStructPtrin, char* inString, int VarSelectionIn, struct SPDStruct* DataStructArray);

#ifdef __cplusplus

class packetsAPIClass // declaration of console menu class
{
protected:
    struct packAPIStruct* packDataPtr = nullptr;
public:
    packetsAPIClass(struct packAPIStruct* packDataPtrIn);
    struct packAPIStruct* getPacketdataPtr();
};

// make it a base class instead
class SPDClass
{
private:
    int VarIndex;
    struct SPDStruct* SPDArray;

public:
    SPDClass(int VarIndexIn, struct SPDStruct* SPDArrayPtr);

    float getFloatValue();

    struct SPDStruct* getSPDArray();
    void* getDataPtr();
    int getVarIndex();

    virtual const char* getLabelString() = 0;
    virtual const char* getUnitsString() = 0;
};

#endif // !__cplusplus

#endif // ! __PACKETS_API__
