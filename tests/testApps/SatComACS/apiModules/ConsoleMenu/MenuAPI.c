/** \file MenuAPI.c
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

#include "LCDapi.h"

void writeUIHelpString(struct uiStruct* uiStructPtrIn)
{
	if (uiStructPtrIn->showHelp == ui8TRUE)
	{
		OPENSWITCH(uiStructPtrIn)
        case 0:
			PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), xstr(Mn), "Help", terminalSlashes()   END_MENU_LN;
		case 1:
			PRINT_MENU_LN  "\n%sSatComACS; \tto display Main Menu", cursorString(0)   END_MENU_LN;
		case 2:
			PRINT_MENU_LN  "\n%sTerminal; \tto display Terminal Menu", cursorString(0)    END_MENU_LN;
		case 3:
			PRINT_MENU_LN   "\n%sDevices; \tto display Devices Menu", cursorString(0)   END_MENU_LN;
		case 4:
			PRINT_MENU_LN  "\n%sHelp; \tto display This View", cursorString(0)    END_MENU_LN;
		case 5:
			PRINT_MENU_LN  "\n%sUserLevel:Operator; \n\tto set current user level", cursorString(0)    END_MENU_LN;
		case 6:
			PRINT_MENU_LN  "\n%s; \t\tto Refresh Current View", cursorString(0)    END_MENU_LN;
		case 7:
			PRINT_MENU_LN  "\n%s %s - %s ", terminalSlashes(), xstr(Mn), "Key Pad Help"   END_MENU_LN;
		case 8:
			PRINT_MENU_LN  "\n%s7  Home\t8  Up", cursorString(0)    END_MENU_LN;
		case 9:
			PRINT_MENU_LN  "\n%s4  Back\t5  Action    6  Forward", cursorString(0)   END_MENU_LN;
		case 10:
			PRINT_MENU_LN  "\n%s1  Main\t2  Down", cursorString(0)    END_MENU_LN;
		default:
		CLOSESWITCH(uiStructPtrIn)
		uiStructPtrIn->showHelp = ui8FALSE;
	}
}
const char* terminalPromptString(int userLevelIndex)
{
	switch ((enum uiAccessLevel)userLevelIndex)
	{
	case  uiLevel_Developer: return "\n(Developer)>>";
	case  uiLevel_Administrator: return "\n(Administrator)>>";
	case  uiLevel_Operator: return "\n(Operator)>>";
	case  uiLevel_Observer: return "\n(Observer)>>";
	default: return "\n>>";
	}
}

void parseUserLevel(struct uiStruct* uiStructPtrIn)
{
	
	OPENIF("UserLevel", uiStructPtrIn->currentMenuIndex)
		
		if (stringMatchCaseSensitive(&uiStructPtrIn->devptr->inbuff.charbuff[uiStructPtrIn->parseIndex], "Developer") == ui8TRUE)
			uiStructPtrIn->currentUserLevel = uiLevel_Developer;
		else if (stringMatchCaseSensitive(&uiStructPtrIn->devptr->inbuff.charbuff[uiStructPtrIn->parseIndex], "Operator") == ui8TRUE)
			uiStructPtrIn->currentUserLevel = uiLevel_Operator;
		else if (stringMatchCaseSensitive(&uiStructPtrIn->devptr->inbuff.charbuff[uiStructPtrIn->parseIndex], "Administrator") == ui8TRUE)
			uiStructPtrIn->currentUserLevel = uiLevel_Administrator;
		else if (stringMatchCaseSensitive(&uiStructPtrIn->devptr->inbuff.charbuff[uiStructPtrIn->parseIndex], "Observer") == ui8TRUE)
			uiStructPtrIn->currentUserLevel = uiLevel_Observer;

	CLOSEIF("UserLevel", uiStructPtrIn->currentMenuIndex)
}

void goParsedAction(enum currentMenuAction theCurrentAction, struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	switch (theCurrentAction)
	{
	case cA_Down:uiStructPtrIn->cursorIndex++;		break;
	case cA_Up:uiStructPtrIn->cursorIndex--;		break;
	case cA_Forward:								break;
	case cA_Back:									break;
	case cA_Enter:									break;
	case cA_Status:									break;
	}
}

void parseConsoleKeyPadAPI(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	enum currentMenuAction theCurrentAction = cA_NoAction;
	// Down
	KEYIF("2", cA_Down)
	// Up
	KEYIF("8", cA_Up);
	// Forward
	KEYIF("6", cA_Forward);
	// Back
	KEYIF("4", cA_Back);
	// Enter
	KEYIF("5", cA_Enter);
	// Status
	KEYIF("7", cA_Status);
	goParsedAction(theCurrentAction, satcomacsStructPtrIn, uiStructPtrIn);
	
}
// SatComACS
void parseGroupSatComACS(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	if (uiStructPtrIn == &satcomacsStructPtrIn->ConsoleMenu)
	{
		parseConsoleKeyPadAPI(satcomacsStructPtrIn, uiStructPtrIn);
		parseTerminalMenuAPI(&satcomacsStructPtrIn->Terminal, uiStructPtrIn);
		parseAPTMenuAPI(&satcomacsStructPtrIn->APT, uiStructPtrIn);
		parseTPMMenuAPI(&satcomacsStructPtrIn->TPM, uiStructPtrIn);
		parseTxRxMenuAPI(&satcomacsStructPtrIn->TxRx, uiStructPtrIn);
		parseUserLevel(uiStructPtrIn);
		if (stringMatchCaseSensitive(&uiStructPtrIn->devptr->inbuff.charbuff[uiStructPtrIn->parseIndex], "Help") == ui8TRUE)
			uiStructPtrIn->showHelp = ui8TRUE;
	}
	else if (uiStructPtrIn == &satcomacsStructPtrIn->LCDKeyPad)
	{
		parseLCDKeyPadAPI(satcomacsStructPtrIn, uiStructPtrIn);
	}
	
}
void parseSatComACSMenuAPI(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	//(Status) SatComACS; to see cM_MainMenu screen
	OPENIF("SatComACS", cM_MainMenu)
		//(Status) SatComACS:Terminal:State; 
		//(Command) SatComACS:Terminal:State:antState_init;
		//(Command) SatComACS:APT:GPS:Connected:0;		
		parseGroupSatComACS(satcomacsStructPtrIn, uiStructPtrIn);
	CLOSEIF("SatComACS", cM_MainMenu)
	//(Status) Terminal:State; 
	//(Command) Terminal:State:antState_init;
	//(Command) APT:GPS:Connected:0;		
	parseGroupSatComACS(satcomacsStructPtrIn, uiStructPtrIn);
}
void writeSatComACSMenuScreenConsole(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
				case 0:
					PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), xstr(Mn), "Main", terminalSlashes()   END_MENU_LN;
				case 1:
					PRINT_MENU_LN  "\n%sTerminal", cursorString(uiStructPtrIn->cursorIndex == 0)   END_MENU_LN;
				case 2:
					PRINT_MENU_LN  "\n%sDevices", cursorString(uiStructPtrIn->cursorIndex == 1)    END_MENU_LN;
				case 3:
					PRINT_MENU_LN  "\n%sExecution System", cursorString(uiStructPtrIn->cursorIndex == 2)    END_MENU_LN;
				default:
					CLOSESWITCH(uiStructPtrIn)
}
void writeSatComACSMenuScreenLCDBig(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
				case 0:
					PRINT_MENU_LN  "\n%s - %s Menu", xstr(Mn), "Main"   END_MENU_LN;
				case 1:
					PRINT_MENU_LN  "\n%sTerminal", cursorString(uiStructPtrIn->cursorIndex == 0)   END_MENU_LN;
				case 2:
					PRINT_MENU_LN  "\n%sDevices", cursorString(uiStructPtrIn->cursorIndex == 1)    END_MENU_LN;
				default:
					CLOSESWITCH(uiStructPtrIn)
}
void writeSatComACSMenuScreenLCDSmall(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
				case 0:
					PRINT_MENU_LN  "\n%sTerminal", cursorString(uiStructPtrIn->cursorIndex == 0)   END_MENU_LN;
				case 1:
					PRINT_MENU_LN  "\n%sDevices", cursorString(uiStructPtrIn->cursorIndex == 1)    END_MENU_LN;
				default:
					CLOSESWITCH(uiStructPtrIn)
}
void writeSatComACSMenuScreen(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	switch (uiStructPtrIn->viewFormatIndex)
	{
	case cV_Console: writeSatComACSMenuScreenConsole(satcomacsStructPtrIn, uiStructPtrIn); break;
	case cv_LCD4_40: writeSatComACSMenuScreenLCDBig(satcomacsStructPtrIn, uiStructPtrIn); break;
	case cv_LCD2_40: writeSatComACSMenuScreenLCDSmall(satcomacsStructPtrIn, uiStructPtrIn); break;
	}
}

// SatComACS:Devices
void parseSatComACSDevicesMenuAPI(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	parseSatComACSMenuAPI(satcomacsStructPtrIn, uiStructPtrIn);
}
void writeSatComACSDevicesMenuScreenConsole(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
			case 0:
				PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), xstr(Mn), "Devices", terminalSlashes()   END_MENU_LN;
			case 1:
				PRINT_MENU_LN  "\n%sAPT WMM Device", cursorString(uiStructPtrIn->cursorIndex == 0)		END_MENU_LN;
			case 2:
				PRINT_MENU_LN  "\n%sTPM Device", cursorString(uiStructPtrIn->cursorIndex == 1)			END_MENU_LN;
			case 3:
				PRINT_MENU_LN  "\n%sTxRx Device", cursorString(uiStructPtrIn->cursorIndex == 2)		END_MENU_LN;
			default:
				CLOSESWITCH(uiStructPtrIn)
}
void writeSatComACSDevicesMenuScreenLCDBig(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
			case 0:
				PRINT_MENU_LN  "\n%s - %s Menu ", xstr(Mn), "Devices"   END_MENU_LN;
			case 1:
				PRINT_MENU_LN  "\n%sAPT WMM Device", cursorString(uiStructPtrIn->cursorIndex == 0)		END_MENU_LN;
			case 2:
				PRINT_MENU_LN  "\n%sTPM Device", cursorString(uiStructPtrIn->cursorIndex == 1)			END_MENU_LN;
			case 3:
				PRINT_MENU_LN  "\n%sTxRx Device", cursorString(uiStructPtrIn->cursorIndex == 2)		END_MENU_LN;
			default:
				CLOSESWITCH(uiStructPtrIn)
}
void writeSatComACSDevicesMenuScreenLCDSmall(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
			case 0:
				PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), xstr(Mn), "Devices", terminalSlashes()   END_MENU_LN;
			case 1:
				PRINT_MENU_LN  "\n%sAPT WMM Device", cursorString(uiStructPtrIn->cursorIndex == 0)		END_MENU_LN;
			default:
				CLOSESWITCH(uiStructPtrIn)
}
void writeSatComACSDevicesMenuScreen(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	switch (uiStructPtrIn->viewFormatIndex)
	{
	case cV_Console: writeSatComACSDevicesMenuScreenConsole(satcomacsStructPtrIn, uiStructPtrIn); break;
	case cv_LCD4_40: writeSatComACSDevicesMenuScreenLCDBig(satcomacsStructPtrIn, uiStructPtrIn); break;
	case cv_LCD2_40: writeSatComACSDevicesMenuScreenLCDSmall(satcomacsStructPtrIn, uiStructPtrIn); break;
	}
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
void writeTerminalMenuScreenConsole(struct antennaStruct* terminalStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
	case 0:
		PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), xstr(Mn), "Terminal", terminalSlashes()   END_MENU_LN;
	case 1:
		PRINT_MENU_LN  "\nCurrent State: %s", currentStateString(terminalStructPtrIn)						END_MENU_LN;
	case 2:
		PRINT_MENU_LN  "\n%sGeo-Location", cursorString(uiStructPtrIn->cursorIndex == 0)					END_MENU_LN;
	case 3:
		PRINT_MENU_LN  "\n%sAttitude and Position", cursorString(uiStructPtrIn->cursorIndex == 1)			END_MENU_LN;
	case 4:
		PRINT_MENU_LN  "\n%sSatCom System", cursorString(uiStructPtrIn->cursorIndex == 2)					END_MENU_LN;
	default:
		CLOSESWITCH(uiStructPtrIn)
}
void writeTerminalMenuScreenLCDSmall(struct antennaStruct* terminalStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
	case 0:
		PRINT_MENU_LN  "\n%s - %s Menu ", xstr(Mn), "Terminal"   END_MENU_LN;
	case 1:
		PRINT_MENU_LN  "\nCurrent State: %s", currentStateString(terminalStructPtrIn)						END_MENU_LN;
	case 2:
		PRINT_MENU_LN  "\n%sGeo-Location", cursorString(uiStructPtrIn->cursorIndex == 0)					END_MENU_LN;
	case 3:
		PRINT_MENU_LN  "\n%sAttitude and Position", cursorString(uiStructPtrIn->cursorIndex == 1)			END_MENU_LN;
	default:
		CLOSESWITCH(uiStructPtrIn)
}
void writeTerminalMenuScreenLCDBig(struct antennaStruct* terminalStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
	case 0:
		PRINT_MENU_LN  "\n%s - %s Menu ", xstr(Mn), "Terminal"   END_MENU_LN;
	case 1:
		PRINT_MENU_LN  "\nCurrent State: %s", currentStateString(terminalStructPtrIn)						END_MENU_LN;
	default:
		CLOSESWITCH(uiStructPtrIn)
}
void writeTerminalMenuScreen(struct antennaStruct* terminalStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	switch (uiStructPtrIn->viewFormatIndex)
	{
	case cV_Console: writeTerminalMenuScreenConsole(terminalStructPtrIn, uiStructPtrIn); break;
	case cv_LCD4_40: writeTerminalMenuScreenLCDBig(terminalStructPtrIn, uiStructPtrIn); break;
	case cv_LCD2_40: writeTerminalMenuScreenLCDSmall(terminalStructPtrIn, uiStructPtrIn); break;
	}
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
void writeTxRxMenuScreenConsole(struct txRxStruct* txRxStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
	case 0:
		PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), xstr(Mn), "TxRx", terminalSlashes()   END_MENU_LN;
	case 1:
		PRINT_MENU_LN  "\nCurrent Attenuation Tx1: %6.2f dB", txRxStructPtrIn->AttenuatorValues[0]      END_MENU_LN;
	case 3:
		PRINT_MENU_LN  "\nCurrent Attenuation Rx:  %6.2f dB", txRxStructPtrIn->AttenuatorValues[1]      END_MENU_LN;
	case 2:
		PRINT_MENU_LN  "\nCurrent Attenuation Tx2: %6.2f dB", txRxStructPtrIn->AttenuatorValues[2]      END_MENU_LN;
	case 4:
		PRINT_MENU_LN  "\n//////////////////////////////////"      END_MENU_LN;
	case 5:
		PRINT_MENU_LN  "\nType \"T1:22.75;\" set Tx1 to 22.75"      END_MENU_LN;
	case 6:
		PRINT_MENU_LN  "\nType \"R:21.00;\"  set Rx  to 21.00"      END_MENU_LN;
	case 7:
		PRINT_MENU_LN  "\nType \"T2:19.25;\" set Tx2 to 19.25"      END_MENU_LN;
	default:
		CLOSESWITCH(uiStructPtrIn)
}
void writeTxRxMenuScreenLCDBig(struct txRxStruct* txRxStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
	case 0:
		PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), xstr(Mn), "TxRx", terminalSlashes()   END_MENU_LN;
	case 1:
		PRINT_MENU_LN  "\nCurrent Attenuation Tx1: %6.2f dB", txRxStructPtrIn->AttenuatorValues[0]      END_MENU_LN;
	case 3:
		PRINT_MENU_LN  "\nCurrent Attenuation Rx:  %6.2f dB", txRxStructPtrIn->AttenuatorValues[1]      END_MENU_LN;
	case 2:
		PRINT_MENU_LN  "\nCurrent Attenuation Tx2: %6.2f dB", txRxStructPtrIn->AttenuatorValues[2]      END_MENU_LN;
	default:
		CLOSESWITCH(uiStructPtrIn)
}
void writeTxRxMenuScreenLCDSmall(struct txRxStruct* txRxStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
	case 1:
		PRINT_MENU_LN  "\nCurrent Attenuation Tx1: %6.2f dB", txRxStructPtrIn->AttenuatorValues[0]      END_MENU_LN;
		//case 3:
		//	PRINT_MENU_LN  "\nCurrent Attenuation Rx:  %6.2f dB", txRxStructPtrIn->AttenuatorValues[1]      END_MENU_LN;
	case 2:
		PRINT_MENU_LN  "\nCurrent Attenuation Tx2: %6.2f dB", txRxStructPtrIn->AttenuatorValues[2]      END_MENU_LN;
	default:
		CLOSESWITCH(uiStructPtrIn)
}
void writeTxRxMenuScreen(struct txRxStruct* txRxStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	switch (uiStructPtrIn->viewFormatIndex)
	{
	case cV_Console: writeTxRxMenuScreenConsole(txRxStructPtrIn, uiStructPtrIn); break;
	case cv_LCD4_40: writeTxRxMenuScreenLCDBig(txRxStructPtrIn, uiStructPtrIn); break;
	case cv_LCD2_40: writeTxRxMenuScreenLCDSmall(txRxStructPtrIn, uiStructPtrIn); break;
	}
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
void writeAPTMenuScreenConsole(struct aptStruct* aptStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
	case 0:
		PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), xstr(Mn), "APT", terminalSlashes()	END_MENU_LN;
	case 1:
		PRINT_MENU_LN  "\nLatitude: \t\t%8.4f", aptStructPtrIn->GPS.data.lattitude						END_MENU_LN;
	case 2:
		PRINT_MENU_LN  "\nLongitude: \t\t%8.4f", aptStructPtrIn->GPS.data.longitude						END_MENU_LN;
	case 3:
		PRINT_MENU_LN  "\nAltitude: \t\t%8.0f", aptStructPtrIn->GPS.data.altitude						END_MENU_LN;
	case 4:
		PRINT_MENU_LN  "\nDate/Time (UTC): %2i/%2i/%4i %6.0f", aptStructPtrIn->GPS.data.month, aptStructPtrIn->GPS.data.day, aptStructPtrIn->GPS.data.year, aptStructPtrIn->GPS.data.utctime  END_MENU_LN;
	case 5:
		PRINT_MENU_LN  "\nMagnetic Declination: \t%6.2f", 0.0											END_MENU_LN;
	case 6:
		PRINT_MENU_LN  "\nAzimuth Heading: \t%6.2f", aptStructPtrIn->eCompass.data.yaw					END_MENU_LN;
	case 7:
		PRINT_MENU_LN  "\nElevation Pitch: \t%6.2f", aptStructPtrIn->eCompass.data.pitch				END_MENU_LN;
	case 8:
		PRINT_MENU_LN  "\nAPT Roll: \t\t%6.2f", aptStructPtrIn->eCompass.data.roll						END_MENU_LN;
	default:
		CLOSESWITCH(uiStructPtrIn)
}
void writeAPTMenuScreenLCDBig(struct aptStruct* aptStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
	case 0:
		PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), xstr(Mn), "APT", terminalSlashes()	END_MENU_LN;
	case 1:
		PRINT_MENU_LN  "\nLatitude: \t\t%8.4f", aptStructPtrIn->GPS.data.lattitude						END_MENU_LN;
	case 2:
		PRINT_MENU_LN  "\nLongitude: \t\t%8.4f", aptStructPtrIn->GPS.data.longitude						END_MENU_LN;
	case 3:
		PRINT_MENU_LN  "\nAltitude: \t\t%8.0f", aptStructPtrIn->GPS.data.altitude						END_MENU_LN;
	default:
		CLOSESWITCH(uiStructPtrIn)
}
void writeAPTMenuScreenLCDSmall(struct aptStruct* aptStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
	case 0:
		PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), xstr(Mn), "APT", terminalSlashes()	END_MENU_LN;
	case 1:
		PRINT_MENU_LN  "\nLatitude: \t\t%8.4f", aptStructPtrIn->GPS.data.lattitude						END_MENU_LN;
	default:
		CLOSESWITCH(uiStructPtrIn)
}
void writeAPTMenuScreen(struct aptStruct* aptStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	switch (uiStructPtrIn->viewFormatIndex)
	{
	case cV_Console: writeAPTMenuScreenConsole(aptStructPtrIn, uiStructPtrIn); break;
	case cv_LCD4_40: writeAPTMenuScreenLCDBig(aptStructPtrIn, uiStructPtrIn); break;
	case cv_LCD2_40: writeAPTMenuScreenLCDSmall(aptStructPtrIn, uiStructPtrIn); break;
	}
	
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
void writeTPMMenuScreenConsole(struct tpmStruct* tpmStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
	case 0:
		PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), xstr(Mn), "TPM", terminalSlashes()   END_MENU_LN;
	case 1:
		PRINT_MENU_LN  "\nDesired Center   (MHz): %9.4f", tpmStructPtrIn->freqConverter.data.DesiredCenterFreqMHz			END_MENU_LN;
	case 2:
		PRINT_MENU_LN  "\nRequired LO      (MHz): %9.4f", tpmStructPtrIn->freqConverter.data.RequiredLOFreqMHz	END_MENU_LN;
	case 3:
		PRINT_MENU_LN  "\nLocked on LO     (0/1):    %1d", tpmStructPtrIn->freqConverter.data.LockedOnRequiredLOFreq						END_MENU_LN;
	case 4:
		PRINT_MENU_LN  "\nPower Filter BW  (MHz):  %3.0f", tpmStructPtrIn->powerMeter.data.BandwidthMHz  END_MENU_LN;
	case 5:
		PRINT_MENU_LN  "\nPower In-Band    (dBm):  %6.2f", tpmStructPtrIn->powerMeter.data.PowerMeasuredinBanddB 		END_MENU_LN;
	case 6:
		PRINT_MENU_LN  "\nADC Reading Good (0/1):    %1d", tpmStructPtrIn->powerMeter.data.value_good					END_MENU_LN;
	case 7:
		PRINT_MENU_LN  "\nADC Value             : %4d", tpmStructPtrIn->powerMeter.data.current_value				END_MENU_LN;
	default:
		CLOSESWITCH(uiStructPtrIn)
}
void writeTPMMenuScreenLCDBig(struct tpmStruct* tpmStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
	case 0:
		PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), xstr(Mn), "TPM", terminalSlashes()   END_MENU_LN;
	case 1:
		PRINT_MENU_LN  "\nDesired Center   (MHz): %9.4f", tpmStructPtrIn->freqConverter.data.DesiredCenterFreqMHz			END_MENU_LN;
	case 2:
		PRINT_MENU_LN  "\nRequired LO      (MHz): %9.4f", tpmStructPtrIn->freqConverter.data.RequiredLOFreqMHz	END_MENU_LN;
	case 3:
		PRINT_MENU_LN  "\nLocked on LO     (0/1):    %1d", tpmStructPtrIn->freqConverter.data.LockedOnRequiredLOFreq						END_MENU_LN;
	default:
		CLOSESWITCH(uiStructPtrIn)
}
void writeTPMMenuScreenLCDSmall(struct tpmStruct* tpmStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn)
	case 0:
		PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), xstr(Mn), "TPM", terminalSlashes()   END_MENU_LN;
	case 1:
		PRINT_MENU_LN  "\nDesired Center   (MHz): %9.4f", tpmStructPtrIn->freqConverter.data.DesiredCenterFreqMHz			END_MENU_LN;
	default:
		CLOSESWITCH(uiStructPtrIn)
}
void writeTPMMenuScreen(struct tpmStruct* tpmStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	
	switch (uiStructPtrIn->viewFormatIndex)
	{
	case cV_Console: writeTPMMenuScreenConsole(tpmStructPtrIn, uiStructPtrIn); break;
	case cv_LCD4_40: writeTPMMenuScreenLCDBig(tpmStructPtrIn, uiStructPtrIn); break;
	case cv_LCD2_40: writeTPMMenuScreenLCDSmall(tpmStructPtrIn, uiStructPtrIn); break;
	}
	
}
