/** \file TxRxmodule.c
*   \brief part of ccNOos, Implementation for straight C, or Declarations for C++ wrappers 

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

#include "TxRxmodule.h"


struct txRxStruct createtxRxStruct()
{
    struct txRxStruct outStruct;
    int i;
    for (i = 0; i < MAX_NUM_ATTENUATORS; i++)
    {
        outStruct.AttenuatorNeedsWriting[i] = ui8FALSE;
    }    
    outStruct.INDEX_Attenuator = 0;
    outStruct.CMD_AttenuatorBits = 0x00;
    return outStruct;
}