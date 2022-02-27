/** \file SatComACS_Menu.c
*   \brief Cross-Platform Portable ccNOos Tests Definitions

   Copyright 2021 InMechaSol, Inc

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

Notes:

This source demonstrates the usage of the ccNOos library and its dependence
on other files in the library.  A platform specific main(.c,.cpp) file need
only instantiate, configure, then execute the execution system.

*/
#include "MenuAPI.h"

// SatComACS
void parseSatComACSMenuAPI(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	//(Status) SatComACS; to see cM_MainMenu screen
	OPENIF("SatComACS", cM_MainMenu)
		//(Status) SatComACS:Terminal:State; 
		//(Command) SatComACS:Terminal:State:antState_init;
		//(Command) SatComACS:APT:GPS:Connected:0;		
		parseTerminalMenuAPI(&satcomacsStructPtrIn->Terminal, uiStructPtrIn);
		parseAPTMenuAPI(&satcomacsStructPtrIn->APT, uiStructPtrIn);
		parseTPMMenuAPI(&satcomacsStructPtrIn->TPM, uiStructPtrIn);
		parseTxRxMenuAPI(&satcomacsStructPtrIn->TxRx, uiStructPtrIn);
	CLOSEIF("SatComACS", cM_MainMenu)
}
void writeSatComACSMenuScreen(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn->linesprinted)
		case 0:
			PRINT_MENU_LN  "\n/////////// SatComACS - Main Menu ///////////////////"			END_MENU_LN;
		case 1:
			PRINT_MENU_LN  "\n%sTerminal", cursorString(uiStructPtrIn->currentMenuIndex == 0)   END_MENU_LN;
		case 2:
			PRINT_MENU_LN  "\n%sDevices", cursorString(uiStructPtrIn->currentMenuIndex == 1)    END_MENU_LN;
		default:
	CLOSESWITCH(uiStructPtrIn->linesprinted)
}

// SatComACS:Devices
void parseSatComACSDevicesMenuAPI(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	parseSatComACSMenuAPI(satcomacsStructPtrIn, uiStructPtrIn);
}
void writeSatComACSDevicesMenuScreen(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn->linesprinted)
		case 0:
			PRINT_MENU_LN  "\n/////////// SatComACS - Devices Menu ///////////////////"					END_MENU_LN;
		case 1:
			PRINT_MENU_LN  "\n%sAPT WMM Device", cursorString(uiStructPtrIn->currentMenuIndex == 0)		END_MENU_LN;
		case 2:
			PRINT_MENU_LN  "\n%sTPM Device", cursorString(uiStructPtrIn->currentMenuIndex == 1)			END_MENU_LN;
		case 3:
			PRINT_MENU_LN  "\n%sTxRx Device", cursorString(uiStructPtrIn->currentMenuIndex == 2)		END_MENU_LN;
		default:
	CLOSESWITCH(uiStructPtrIn->linesprinted)
}

// SatComControl: antennaStruct
const char* currentStateString(struct antennaStruct* terminalStructPtrIn)
{
	switch (terminalStructPtrIn->State)
	{
	case antState_init: return "Initialization";
	case antState_NotAcquired: return "Not Acquiring";
	case antState_Acquiring: return "Acquiring";
	case antState_Acquired: return "Acquired";
	case antState_Error: return "Error";
	default: return "Invalid State!";
	}
}
void parseTerminalMenuAPI(struct antennaStruct* terminalStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	//(Status) Terminal; to see cM_Terminal screen
	OPENIF("Terminal", cM_Terminal)	
		//(Status) Terminal:State; 
		//(Command) Terminal:State:antState_init;
		//(Command) Terminal:TargetSatellite:UpLinkFreq_kHz:2100000.0;		
	CLOSEIF("Terminal", cM_Terminal)
}
void writeTerminalMenuScreen(struct antennaStruct* terminalStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn->linesprinted)
		case 0:
			PRINT_MENU_LN  "\n/////////// SatComACS - Terminal Menu ///////////////////"    END_MENU_LN;
		case 1:
			PRINT_MENU_LN  "\nCurrent State: %s", currentStateString(terminalStructPtrIn)    END_MENU_LN;
		case 2:
			PRINT_MENU_LN  "\n%sGeo-Location", cursorString(uiStructPtrIn->currentMenuIndex == 0)    END_MENU_LN;
		case 3:
			PRINT_MENU_LN  "\n%sAttitude and Position", cursorString(uiStructPtrIn->currentMenuIndex == 1)    END_MENU_LN;
		case 4:
			PRINT_MENU_LN  "\n%sSatCom System", cursorString(uiStructPtrIn->currentMenuIndex == 2)    END_MENU_LN;
		default:
			CLOSESWITCH(uiStructPtrIn->linesprinted)

}

// TxRx Module
void parseTxRxMenuAPI(struct txRxStruct* txRxStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	//(Status) TxRx; to see cM_TxRx screen
	OPENIF("TxRx", cM_devTXRX)
		//(Status) TxRx:; 
		//(Command) TxRx::;
		//(Command) TxRx:::;		
	CLOSEIF("TxRx", cM_devTXRX)
}
void writeTxRxMenuScreen(struct txRxStruct* txRxStructPtrIn, struct uiStruct* uiStructPtrIn)
{

	OPENSWITCH(uiStructPtrIn->linesprinted)
		case 0:
			PRINT_MENU_LN  "\n/////////// SatComACS - Tx/Rx Menu ///////////////////"    END_MENU_LN;
		default:
	CLOSESWITCH(uiStructPtrIn->linesprinted)

}

// APT Module
void parseAPTMenuAPI(struct aptStruct* aptStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	//(Status) APT; to see cM_TxRx screen
	OPENIF("APT", cM_devAPT)
		//(Status) APT:; 
		//(Command) APT::;
		//(Command) APT:::;		
	CLOSEIF("APT", cM_devAPT)
}
void writeAPTMenuScreen(struct aptStruct* aptStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn->linesprinted)
		case 0:
			PRINT_MENU_LN  "\n/////////// SatComACS - APT Menu ///////////////////"    END_MENU_LN;
		default:
	CLOSESWITCH(uiStructPtrIn->linesprinted)
}

// TPM Module
void parseTPMMenuAPI(struct tpmStruct* tpmStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	//(Status) APT; to see cM_TxRx screen
	OPENIF("TPM", cM_devTPM)
		//(Status) TPM:; 
		//(Command) TPM::;
		//(Command) TPM:::;		
	CLOSEIF("TPM", cM_devTPM)
}
void writeTPMMenuScreen(struct tpmStruct* tpmStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn->linesprinted)
		case 0:
			PRINT_MENU_LN  "\n/////////// SatComACS - TPM Menu ///////////////////"    END_MENU_LN;
		default:
	CLOSESWITCH(uiStructPtrIn->linesprinted)
}