/** \file packets_api.c
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

#include "packets_api.h"
#include "execution_system.h"

struct packAPIStruct createpackAPIStruct()
{
    struct packAPIStruct outStruct;
    outStruct.devptr = nullptr;
    outStruct.inPackID = 0;
    outStruct.outPackID = 0;
    outStruct.inPackLen = 0;
    outStruct.outPackLen = 0;
    outStruct.inPackNum = 0;
    outStruct.outPackNum = 0;
    outStruct.inPackType = 0;
    outStruct.outPackType = 0;
    outStruct.inPackCounter = 0;
    outStruct.outPackCounter = 0;
    outStruct.inPackBytesCounter = 0;
    outStruct.outPackBytesCounter = 0;
    outStruct.Connected = ui8FALSE;
    outStruct.resetStats = ui8FALSE;

    return outStruct;
}

void parseHeader(struct packAPIStruct* packStructPtrIn)
{
    packStructPtrIn->inPackNum = packStructPtrIn->devptr->inbuff.bytebuff[packStructPtrIn->devptr->parseIndex++];
    packStructPtrIn->inPackID = packStructPtrIn->devptr->inbuff.bytebuff[packStructPtrIn->devptr->parseIndex++];
    packStructPtrIn->inPackType = packStructPtrIn->devptr->inbuff.bytebuff[packStructPtrIn->devptr->parseIndex++];
    packStructPtrIn->inPackLen = packStructPtrIn->devptr->inbuff.bytebuff[packStructPtrIn->devptr->parseIndex++];
}
void packageHeader(struct packAPIStruct* packStructPtrIn)
{
    packStructPtrIn->devptr->numbytes2Write = 0;
    packStructPtrIn->devptr->outbuff.bytebuff[packStructPtrIn->devptr->numbytes2Write++] = packStructPtrIn->outPackNum;
    packStructPtrIn->devptr->outbuff.bytebuff[packStructPtrIn->devptr->numbytes2Write++] = packStructPtrIn->outPackID;
    packStructPtrIn->devptr->outbuff.bytebuff[packStructPtrIn->devptr->numbytes2Write++] = packStructPtrIn->outPackType;
    packStructPtrIn->devptr->outbuff.bytebuff[packStructPtrIn->devptr->numbytes2Write++] = packStructPtrIn->outPackLen;
}

void packageToken(struct packAPIStruct* packStructPtrIn, void* TokenPtr, UI_8 TokenLen)
{
    int i;
    for(i = 0; i < TokenLen; i++)
        packStructPtrIn->devptr->outbuff.bytebuff[packStructPtrIn->devptr->numbytes2Write++] = ((UI_8*)TokenPtr)[i];
}
void packageTokenEndianSwapped(struct packAPIStruct* packStructPtrIn, void* TokenPtr, UI_8 TokenLen)
{
    int i;
    for (i = 0; i < TokenLen; i++)
        packStructPtrIn->devptr->outbuff.bytebuff[packStructPtrIn->devptr->numbytes2Write++] = ((UI_8*)TokenPtr)[TokenLen-1-i];
}

void parseToken(struct packAPIStruct* packStructPtrIn, void* TokenPtr, UI_8 TokenLen)
{
    int i;
    for (i = 0; i < TokenLen; i++)
        ((UI_8*)TokenPtr)[i] = packStructPtrIn->devptr->inbuff.bytebuff[packStructPtrIn->devptr->parseIndex++];
}
void parseTokenEndianSwapped(struct packAPIStruct* packStructPtrIn, void* TokenPtr, UI_8 TokenLen)
{
    int i;
    for (i = 0; i < TokenLen; i++)
        ((UI_8*)TokenPtr)[TokenLen - 1 - i] = packStructPtrIn->devptr->inbuff.bytebuff[packStructPtrIn->devptr->parseIndex++];
}

void parseSPD(struct packAPIStruct* packStructPtrin, struct SPDStruct* DataStructPtr)
{
    // header bytes must have been parsed and values stored in the header structure
    // BEFORE parsing an SPD from the data segment

    // then
    if (packStructPtrin->devptr->swapByteOrder)
    {
        parseTokenEndianSwapped(packStructPtrin, DataStructPtr->addr, DataStructPtr->size);
    }
    else
    {
        parseToken(packStructPtrin, DataStructPtr->addr, DataStructPtr->size);
    }

}
void packageSPD(struct packAPIStruct* packStructPtrin, struct SPDStruct* DataStructPtr)
{
    // header bytes must be set in the packet structure and packaged for transmisstion
    // BEFORE packaging an SPD in the data segment

    // then
    if(packStructPtrin->devptr->swapByteOrder)
    {
        packageTokenEndianSwapped(packStructPtrin, DataStructPtr->addr, DataStructPtr->size);
    }
    else
    {
        packageToken(packStructPtrin, DataStructPtr->addr, DataStructPtr->size);
    }

}

void parseSPDArray(struct packAPIStruct* packStructPtrin, struct SPDStruct* DataStructArray, UI_8 ArrayLen)
{
    // header bytes must have been parsed and values stored in the header structure
    // BEFORE parsing an SPD from the data segment

    // then
    for (int i = 0; i < ArrayLen; i++)
    {
        if (packStructPtrin->devptr->swapByteOrder)
        {
            parseTokenEndianSwapped(packStructPtrin, DataStructArray[i].addr, DataStructArray[i].size);
        }
        else
        {
            parseToken(packStructPtrin, DataStructArray[i].addr, DataStructArray[i].size);
        }
    }
}
void packageSPDArray(struct packAPIStruct* packStructPtrin, struct SPDStruct* DataStructArray, UI_8 ArrayLen)
{
    // header bytes must be set in the packet structure and packaged for transmisstion
    // BEFORE packaging an SPD in the data segment

    // then
    for (int i = 0; i < ArrayLen; i++)
    {
        if (packStructPtrin->devptr->swapByteOrder)
        {
            packageTokenEndianSwapped(packStructPtrin, DataStructArray[i].addr, DataStructArray[i].size);
        }
        else
        {
            packageToken(packStructPtrin, DataStructArray[i].addr, DataStructArray[i].size);
        }
    }
}

///////////////////////////////////////////////
// SPD - Serial Parameter Data (type)
const char* getSPDLabelString(int VarSelectionIn, struct SPDStruct* DataStructArray) { return DataStructArray[VarSelectionIn].labelString; }
const char* getSPDUnitsString(int VarSelectionIn, struct SPDStruct* DataStructArray) { return DataStructArray[VarSelectionIn].unitsString; }
UI_8 getSPDReadOnly(int VarSelectionIn, struct SPDStruct* DataStructArray) { return DataStructArray[VarSelectionIn].readonly; }
void setSPDFloatValue(float inVal, int VarSelectionIn, struct SPDStruct* DataStructArray)
{
    if (DataStructArray[VarSelectionIn].type == FLOAT_TYPE) // floating point
    {
        if (DataStructArray[VarSelectionIn].size == 4)
            *((float*)DataStructArray[VarSelectionIn].addr) = inVal;
        else if (DataStructArray[VarSelectionIn].size == 8)
            *((double*)DataStructArray[VarSelectionIn].addr) = inVal;
    }
    else if (DataStructArray[VarSelectionIn].type == SIGNED_TYPE) // signed integer
    {
        if (DataStructArray[VarSelectionIn].size == 1)
            *((I_8*)DataStructArray[VarSelectionIn].addr) = inVal;
        else if (DataStructArray[VarSelectionIn].size == 2)
            *((I_16*)DataStructArray[VarSelectionIn].addr) = inVal;
        else if (DataStructArray[VarSelectionIn].size == 4)
            *((I_32*)DataStructArray[VarSelectionIn].addr) = inVal;
    }
    else if (DataStructArray[VarSelectionIn].type == UNSIGNED_TYPE) // unsigned integer
    {
        if (DataStructArray[VarSelectionIn].size == 1)
            *((UI_8*)DataStructArray[VarSelectionIn].addr) = inVal;
        else if (DataStructArray[VarSelectionIn].size == 2)
            *((UI_16*)DataStructArray[VarSelectionIn].addr) = inVal;
        else if (DataStructArray[VarSelectionIn].size == 4)
            *((UI_32*)DataStructArray[VarSelectionIn].addr) = inVal;
    }
}
float getSPDFloatValue(int VarSelectionIn, struct SPDStruct* DataStructArray)
{
    if (DataStructArray[VarSelectionIn].type == FLOAT_TYPE) // floating point
    {
        if (DataStructArray[VarSelectionIn].size == 4)
            return *((float*)DataStructArray[VarSelectionIn].addr);
        else if (DataStructArray[VarSelectionIn].size == 8)
            return *((double*)DataStructArray[VarSelectionIn].addr);
    }
    else if (DataStructArray[VarSelectionIn].type == SIGNED_TYPE) // signed integer
    {
        if (DataStructArray[VarSelectionIn].size == 1)
            return *((I_8*)DataStructArray[VarSelectionIn].addr);
        else if (DataStructArray[VarSelectionIn].size == 2)
            return *((I_16*)DataStructArray[VarSelectionIn].addr);
        else if (DataStructArray[VarSelectionIn].size == 4)
            return *((I_32*)DataStructArray[VarSelectionIn].addr);
    }
    else if (DataStructArray[VarSelectionIn].type == UNSIGNED_TYPE) // unsigned integer
    {
        if (DataStructArray[VarSelectionIn].size == 1)
            return *((UI_8*)DataStructArray[VarSelectionIn].addr);
        else if (DataStructArray[VarSelectionIn].size == 2)
            return *((UI_16*)DataStructArray[VarSelectionIn].addr);
        else if (DataStructArray[VarSelectionIn].size == 4)
            return *((UI_32*)DataStructArray[VarSelectionIn].addr);
    }
    return 0;
}

// use temp variable on stack to attempt to parse string value
// into the the SPD.addr
UI_8 setSPDFromString(char* inString, struct SPDStruct* DataStructPtr)
{
    // from size and type determine which parse function to call
        // call the parse function
    UI_8 goodParse = ui8FALSE;
    switch (DataStructPtr->type)
    {
    case UNSIGNED_TYPE:
        switch (DataStructPtr->size)
        {
        case sizeof(union SPD8) : goodParse = ATO_U8(inString,(UI_8*)DataStructPtr->addr);    break;
        case sizeof(union SPD16) : goodParse = ATO_U16(inString, (UI_16*)DataStructPtr->addr);    break;
        case sizeof(union SPD32) : goodParse = ATO_U32(inString, (UI_32*)DataStructPtr->addr);    break;
        case sizeof(union SPD64) : goodParse = ATO_U64(inString, (UI_64*)DataStructPtr->addr);    break;
        }
        break;
    case SIGNED_TYPE: 
        switch (DataStructPtr->size)
        {
        case sizeof(union SPD8) : goodParse = ATO_I8(inString,(I_8*)DataStructPtr->addr);    break;
        case sizeof(union SPD16) : goodParse = ATO_I16(inString, (I_16*)DataStructPtr->addr);    break;
        case sizeof(union SPD32) : goodParse = ATO_I32(inString, (I_32*)DataStructPtr->addr);    break;
        case sizeof(union SPD64) : goodParse = ATO_I64(inString, (I_64*)DataStructPtr->addr);    break;
        }
        break;
    case FLOAT_TYPE: 
        switch (DataStructPtr->size)
        {
        case sizeof(union SPD32) : goodParse = ATO_F(inString, (float*)DataStructPtr->addr);    break;
        case sizeof(union SPD64) : goodParse = ATO_D(inString, (double*)DataStructPtr->addr);    break;
        }
        break;
    }
    
    // return pass/fail
    return goodParse;
}

// use temp variable on stack to hopefully receive parsed string value
// then package parsed string value into SPD packet buffer
UI_8 packageSPDFromString(struct packAPIStruct* packStructPtrin, const char* inString, int VarSelectionIn, struct SPDStruct* DataStructArray)
{
    // create temp containers
    union SPD8  myTempSPD8;
    union SPD16 myTempSPD16;
    union SPD32 myTempSPD32;
    union SPD64 myTempSPD64;

    // package in temp SPDStruct
    struct SPDStruct myDataStruct;
    myDataStruct.selcector = VarSelectionIn;

    // determine SPD type to parse into
    switch (DataStructArray[VarSelectionIn].size)
    {
        case sizeof(union SPD8)  : myDataStruct.addr = &myTempSPD8;  break;
        case sizeof(union SPD16) : myDataStruct.addr = &myTempSPD16; break;
        case sizeof(union SPD32) : myDataStruct.addr = &myTempSPD32; break;
        case sizeof(union SPD64) : myDataStruct.addr = &myTempSPD64; break;
    default: myDataStruct.addr = nullptr;
    }

    // copy size and type from target SPD to tempSPD
    myDataStruct.size = DataStructArray[VarSelectionIn].size;
    myDataStruct.type = DataStructArray[VarSelectionIn].type;
    
    // attempt to parse into temp SPD
    if (setSPDFromString(inString, &myDataStruct))
    {
        // package parsed value in packet for transmission
        packageSPD(packStructPtrin, &myDataStruct);
        return ui8TRUE;
    }
    else
    {
        // indicate failure
        return ui8FALSE;
    }
    
}
