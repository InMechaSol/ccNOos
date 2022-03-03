/** \file TxRxmodule.h
*   \brief part of <a href="https://github.com/InMechaSol/ccNOos">ccNOos</a>, Declarations for straight C and C++ 

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

#ifndef __TXRX_MOD_H__
#define __TXRX_MOD_H__  

#include "console_menu.h"

// Tx/Rx Module Data Structure
#define MAX_NUM_ATTENUATORS (3u)
#define MIN_ATTEN_VAL (0.0)
#define MAX_ATTEN_VAL (31.75)

struct txRxStruct
{
    UI_8 CMD_AttenuatorBits, INDEX_Attenuator;
    float AttenuatorValues[MAX_NUM_ATTENUATORS];
    UI_8 AttenuatorNeedsWriting[MAX_NUM_ATTENUATORS];
};
struct txRxStruct createtxRxStruct();
void writeAttenuatorValues(struct txRxStruct* txRxStructPtrIn);

#ifdef __USINGCONSOLEMENU

void parseTxRxMenuAPI(struct txRxStruct* txRxStructPtrIn, struct uiStruct* uiStructPtrIn);
void writeTxRxMenuScreen(struct txRxStruct* txRxStructPtrIn, struct uiStruct* uiStructPtrIn);
void writeTxRxLogLine(struct txRxStruct* txRxStructPtrIn, struct logStruct* logStructPtrIn);
void parseTxRxConfigLine(struct txRxStruct* txRxStructPtrIn, struct configStruct* configStructPtrIn);

#endif


#ifdef __cplusplus


#endif // !__cplusplus
#endif // !__TXRX_MOD_H__
