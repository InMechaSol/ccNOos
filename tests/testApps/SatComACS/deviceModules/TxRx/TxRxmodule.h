/** \file TxRxmodule.h
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

#ifndef __TXRX_MOD_H__
#define __TXRX_MOD_H__  

#include "console_menu.h"
#include "HMC1119.h"
#include "MCP23017.h"

// Tx/Rx Module Data Structure
#define MAX_NUM_ATTENUATORS (3u)
#define MAX_MDMS (4u)
#define SWITCH_OPEN (0u)

struct txRxStruct
{
    struct DATStruct AttenuatorValues[MAX_NUM_ATTENUATORS];
    UI_8 AttenuatorNeedsWriting[MAX_NUM_ATTENUATORS];
    UI_8 MDMSwitchStates[MAX_MDMS];
    UI_8 MDMSwitchNeedsWriting[MAX_MDMS];
    
};
struct txRxStruct createtxRxStruct(); 
void writeAttenuatorValues(struct txRxStruct* txRxStructPtrIn);
void writeModemSwitchValues(struct txRxStruct* txRxStructPtrIn);

#ifdef __USINGCONSOLEMENU

void parseTxRxMenuAPI(struct txRxStruct* txRxStructPtrIn, struct uiStruct* uiStructPtrIn);
void writeTxRxMenuScreen(struct txRxStruct* txRxStructPtrIn, struct uiStruct* uiStructPtrIn);
void writeTxRxLogLine(struct txRxStruct* txRxStructPtrIn, struct logStruct* logStructPtrIn);
void parseTxRxConfigLine(struct txRxStruct* txRxStructPtrIn, struct configStruct* configStructPtrIn);

#endif

#define __SatComACS_TxRx_WriteFuncsTemplate(DELAYFUNC) \
float ModuloFloat(float floatValue, float* intPartPtr) \
{ \
    double intPart, fracPart; \
    fracPart = modf(floatValue, &intPart); \
    *intPartPtr = (float)intPart; \
    return (float)fracPart; \
} \
void writeAttenuatorValues(struct txRxStruct* txRxStructPtrIn) \
{ \
    for (int i = 0; i < MAX_NUM_ATTENUATORS; i++) { \
        if (txRxStructPtrIn->AttenuatorNeedsWriting[i]) { \
            txRxStructPtrIn->AttenuatorNeedsWriting[i] = ui8FALSE; \
            limitDATcmd(&txRxStructPtrIn->AttenuatorValues[i]); \
            CalcAttenuationBits(&txRxStructPtrIn->AttenuatorValues[i]); \
            switch (i) { \
            case 0: \
                SETPIN_TX_C16(datcmdbit16(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_TX_C8(datcmdbit8(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_TX_C4(datcmdbit4(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_TX_C2(datcmdbit2(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_TX_C1(datcmdbit1(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_TX_C0_25(datcmdbit0_25(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_TX_C0_50(datcmdbit0_50(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_TX_LE(0x01); \
                DELAYFUNC; \
                SETPIN_TX_LE(0x00); \
                break; \
            case 1: \
                SETPIN_RX_C16(datcmdbit16(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_RX_C8(datcmdbit8(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_RX_C4(datcmdbit4(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_RX_C2(datcmdbit2(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_RX_C1(datcmdbit1(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_RX_C0_25(datcmdbit0_25(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_RX_C0_50(datcmdbit0_50(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_RX_LE(0x01); \
                DELAYFUNC; \
                SETPIN_RX_LE(0x00); \
                break; \
            case 2: \
                SETPIN_XX_C16(datcmdbit16(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_XX_C8(datcmdbit8(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_XX_C4(datcmdbit4(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_XX_C2(datcmdbit2(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_XX_C1(datcmdbit1(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_XX_C0_25(datcmdbit0_25(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_XX_C0_50(datcmdbit0_50(txRxStructPtrIn->AttenuatorValues[i].attenSetting)); \
                SETPIN_XX_LE(0x01); \
                DELAYFUNC; \
                SETPIN_XX_LE(0x00); \
                break; \
            default: \
                break; \
            } \
        } \
    } \
} \
void writeModemSwitchValues(struct txRxStruct* txRxStructPtrIn) \
    { \
        for (int i = 0; i < MAX_MDMS; i++) { \
                if (txRxStructPtrIn->MDMSwitchNeedsWriting[i]) { \
                        txRxStructPtrIn->MDMSwitchNeedsWriting[i] = ui8FALSE; \
                        switch (i) { \
                        case 0: \
                                SETPIN_MDM1_SW(txRxStructPtrIn->MDMSwitchStates[i]); \
                                break; \
                        case 1: \
                                SETPIN_MDM2_SW(txRxStructPtrIn->MDMSwitchStates[i]); \
                                break; \
                        case 2: \
                                SETPIN_MDM3_SW(txRxStructPtrIn->MDMSwitchStates[i]); \
                                break; \
                        case 3: \
                                SETPIN_MDM4_SW(txRxStructPtrIn->MDMSwitchStates[i]); \
                                break; \
                        default: \
                                break; \
                        } \
                } \
        } \
    }
#define SatComACS_TxRx_WriteFuncsTemplate(DELAYFUNC) __SatComACS_TxRx_WriteFuncsTemplate(DELAYFUNC)

#ifdef __cplusplus


#endif // !__cplusplus
#endif // !__TXRX_MOD_H__
