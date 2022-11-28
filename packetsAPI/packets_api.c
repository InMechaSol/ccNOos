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

