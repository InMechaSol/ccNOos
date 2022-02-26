#include "PlatformApp_Serialization.h"


void readSatComACSMenuAPI(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	GetMenuChars(uiStructPtrIn);

	if (uiStructPtrIn->devptr->newDataReadIn)
	{
		//(Status) SatComACS;
		//(Status) SatComACS:Terminal:State; 
		//(Command) SatComACS:Terminal:State:antState_init;
		//(Command) SatComACS:LCDKeyPad:currentMenuIndex:cM_MainMenu;
	}
}
void writeSatComACSMenuScreen(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	char* linebuff = &uiStructPtrIn->devptr->outbuff.charbuff[0];
		int charsWritten = 0;
		int maxChars = charBuffMax;

		uiStructPtrIn->linesprinted = 0; 
		uiStructPtrIn->lines2print = 1;
	do {
				
		switch (uiStructPtrIn->linesprinted) {
	//OPENSWITCH(uiStructPtrIn->linesprinted)
		case 0:
			PRINT_MENU_LN  "\n/////////// SatComACS - Main Menu ///////////////////"    END_MENU_LN;
		case 1:
			PRINT_MENU_LN  "\n%sTerminal", cursorString(uiStructPtrIn->currentMenuIndex == 0)    END_MENU_LN;
		case 2:
			PRINT_MENU_LN  "\n%sDevices", cursorString(uiStructPtrIn->currentMenuIndex == 1)    END_MENU_LN;
		default:
	//CLOSESWITCH(uiStructPtrIn->linesprinted)
			uiStructPtrIn->lines2print = 0; 
				break; 
		}
		if (uiStructPtrIn->lines2print > 0)
		{
			WriteMenuLine(uiStructPtrIn); 
			uiStructPtrIn->linesprinted++; 
		}
	} while (uiStructPtrIn->lines2print > 0);
}
void writeSatComACSLogLine(struct SatComACSStruct* satcomacsStructPtrIn, struct logStruct* logStructPtrIn) { ; }
void readSatComACSConfigLine(struct SatComACSStruct* satcomacsStructPtrIn, struct configStruct* configStructPtrIn) { ; }

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
void readTerminalMenuAPI(struct antennaStruct* terminalStructPtrIn, struct uiStruct* uiStructPtrIn) { ; }
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
void writeTerminalLogLine(struct antennaStruct* terminalStructPtrIn, struct logStruct* logStructPtrIn) { ; }
void readTerminalConfigLine(struct antennaStruct* terminalStructPtrIn, struct configStruct* configStructPtrIn) { ; }

void readSatComACSDevicesMenuAPI(struct SatComACSStruct* tpmsatcomacsStructPtrInStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	;
}
void writeSatComACSDevicesMenuScreen(struct SatComACSStruct* tpmsatcomacsStructPtrInStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn->linesprinted)
		case 0:
			PRINT_MENU_LN  "\n/////////// SatComACS - Devices Menu ///////////////////"    END_MENU_LN;
		case 1:
			PRINT_MENU_LN  "\n%sAPT WMM Device", cursorString(uiStructPtrIn->currentMenuIndex==0)    END_MENU_LN;
		case 2:
			PRINT_MENU_LN  "\n%sTPM Device", cursorString(uiStructPtrIn->currentMenuIndex == 1)    END_MENU_LN;
		case 3:
			PRINT_MENU_LN  "\n%sTxRx Device", cursorString(uiStructPtrIn->currentMenuIndex == 2)   END_MENU_LN;
		default:
	CLOSESWITCH(uiStructPtrIn->linesprinted)
}


void readTxRxMenuAPI(struct txRxStruct* txRxStructPtrIn, struct uiStruct* uiStructPtrIn) { ; }
void writeTxRxMenuScreen(struct txRxStruct* txRxStructPtrIn, struct uiStruct* uiStructPtrIn) 
{	
	
	OPENSWITCH(uiStructPtrIn->linesprinted)
		case 0:
			PRINT_MENU_LN  "\n/////////// SatComACS - Tx/Rx Menu ///////////////////"    END_MENU_LN;
		default:
	CLOSESWITCH(uiStructPtrIn->linesprinted)
	
}
void writeTxRxLogLine(struct txRxStruct* txRxStructPtrIn, struct logStruct* logStructPtrIn) { ; }
void readTxRxConfigLine(struct txRxStruct* txRxStructPtrIn, struct configStruct* configStructPtrIn) { ; }


void readAPTMenuAPI(struct aptStruct* aptStructPtrIn, struct uiStruct* uiStructPtrIn) { ; }
void writeAPTMenuScreen(struct aptStruct* aptStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn->linesprinted)
		case 0:
			PRINT_MENU_LN  "\n/////////// SatComACS - APT Menu ///////////////////"    END_MENU_LN;
		default:
	CLOSESWITCH(uiStructPtrIn->linesprinted)
}
void writeAPTLogLine(struct aptStruct* aptStructPtrIn, struct logStruct* logStructPtrIn) { ; }
void readAPTConfigLine(struct aptStruct* aptStructPtrIn, struct configStruct* configStructPtrIn) { ; }


void readTPMMenuAPI(struct tpmStruct* tpmStructPtrIn, struct uiStruct* uiStructPtrIn) { ; }
void writeTPMMenuScreen(struct tpmStruct* tpmStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	OPENSWITCH(uiStructPtrIn->linesprinted)
		case 0:
			PRINT_MENU_LN  "\n/////////// SatComACS - TPM Menu ///////////////////"    END_MENU_LN;
		default:
	CLOSESWITCH(uiStructPtrIn->linesprinted)
}
void writeTPMLogLine(struct tpmStruct* tpmStructPtrIn, struct logStruct* logStructPtrIn) { ; }
void readTPMConfigLine(struct tpmStruct* tpmStructPtrIn, struct configStruct* configStructPtrIn) { ; }



//struct satelliteStruct createSatelliteStructJSON(char* JSONptrIn)
//{
//
//}
//void createJSONSatelliteStruct(struct satelliteStruct* satelliteStructPtrIn)
//{
//
//}
//struct commsParameters createcommsParametersJSON(char* JSONptrIn)
//{
//
//}
//void createJSONcommsParameters(struct commsParameters* commsParametersPtrIn)
//{
//
//}
//struct geoLocationStruct creategeoLocationStructJSON(char* JSONptrIn)
//{
//
//}
//void createJSONgeoLocationStruct(struct geoLocationStruct* geoLocationStructPtrIn)
//{
//
//}
//struct antennaAttitudeStruct createantennaAttitudeStructJSON(char* JSONptrIn)
//{
//
//}
//void createJSONantennaAttitudeStruct(struct antennaAttitudeStruct* antennaAttitudeStructPtrIn)
//{
//
//}
//struct antennaAxis createantennaAxisJSON(char* JSONptrIn)
//{
//
//}
//void createJSONantennaAxis(struct antennaAxis* antennaAxisPtrIn)
//{
//
//}
//struct antennaStruct createantennaStructJSON(char* JSONptrIn)
//{
//
//}
//void createJSONantennaStruct(struct antennaStruct* antennaStructPtrIn)
//{
//
//}