/** \file HMC1119.c
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

#include "HMC1119.h"

struct DATStruct createDATStruct()
{
	struct DATStruct outStruct;
    outStruct.attenSetting = 0;
    outStruct.DATCMD = 0;
	return outStruct;
}

void limitDATcmd(struct DATStruct* DATptr)
{
    float fracPart, intPart;
    if (DATptr->DATCMD < MIN_ATTEN_VAL)
        DATptr->DATCMD = MIN_ATTEN_VAL;
    else if (DATptr->DATCMD > MAX_ATTEN_VAL)
        DATptr->DATCMD = MAX_ATTEN_VAL;

    fracPart = ModuloFloat(DATptr->DATCMD, &intPart);

    if (fracPart > 0.75)
        fracPart = 0.75;
    else if (fracPart < 0.25)
        fracPart = 0.0;
    else if (fracPart > 0.25 && fracPart < 0.50)
        fracPart = 0.25;
    else if (fracPart < 0.75 && fracPart > 0.50)
        fracPart = 0.50;

    DATptr->DATCMD = intPart + fracPart;
}

void CalcAttenuationBits(struct DATStruct* DATptr)
{
    // cmd is limited in elsewhere?

    UI_8 intPart = 0b00011111 & ((UI_8)DATptr->DATCMD);
    float fracPart = DATptr->DATCMD - intPart;

    UI_8 bit16 = (0b00010000 & intPart) >> 4;
    UI_8 bit8 = (0b00001000 & intPart) >> 3;
    UI_8 bit4 = (0b00000100 & intPart) >> 2;
    UI_8 bit2 = (0b00000010 & intPart) >> 1;
    UI_8 bit1 = (0b00000001 & intPart);
    UI_8 bit0_50 = (fracPart > 0.49);
    UI_8 bit0_25 = (fracPart > 0.74);
    if (fracPart < 0.49 && fracPart > 0.0)
        bit0_25 = 1;

    DATptr->attenSetting = (bit16 & 0x01) << 7;
    DATptr->attenSetting |= (bit8 & 0x01) << 6;
    DATptr->attenSetting |= (bit4 & 0x01) << 5;
    DATptr->attenSetting |= (bit2 & 0x01) << 4;
    DATptr->attenSetting |= (bit1 & 0x01) << 3;
    DATptr->attenSetting |= (bit0_50 & 0x01) << 2;
    DATptr->attenSetting |= (bit0_25 & 0x01) << 1;

}

