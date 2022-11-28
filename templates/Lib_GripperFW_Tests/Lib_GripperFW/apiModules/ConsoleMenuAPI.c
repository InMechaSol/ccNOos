/** \file ConsoleMenuAPI.c
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

#include "ConsoleMenuAPI.h"

// Constant String Functions for Menu Display
const char* OpHistoryString(int atIndex)
{
	switch (atIndex)
	{
	case 1: return "Open";
	case 2: return "Close";
	case 3: return "Stop";
	case 4: return "PWM+";
	case 5: return "PWM-";
	case 6: return "PWM0";
	case 7: return "Push";
	case 8: return "Hold";
	default: return " ";
	}
}
const char* gripperModeString(enum enumGripperMode GripperModeIn)
{
	switch (GripperModeIn)
	{
		case gripper_uninitialized: return "uninitialized";
		case gripper_closed: return "closed";
		case gripper_inmotion: return "in motion";
		case gripper_opened: return "opened";
		default: return "invalid mode";
	}
}
const char* vacuumOnOffString(UI_8 vacCmd)
{
	if (vacCmd == ui8TRUE)
		return "On";
	else
		return "Off";
}
const char* cylinderExtendRetractString(UI_8 extendCmd)
{
	if (extendCmd == ui8TRUE)
		return "Extend";
	else
		return "Retract";
}
const char* motionStateString(enum enumSmartMotorState sm_IN, enum brshDriveCtrlMode bldc_IN)
{
	if (bldc_IN == bldc_spinpwm)
	{
		if (sm_IN == sm_stopped)
			return "PWM/X  ";
		else if (sm_IN == sm_stalled)
			return "PWM/O  ";
		else if (sm_IN == sm_positive)
			return "PWM/+  ";
		else if (sm_IN == sm_negative)
			return "PWM/-  ";
		else
			return "PWM/?  ";
	}
	else
	{
		if (sm_IN == sm_stopped)
			return "Coast/X";
		else if (sm_IN == sm_stalled)
			return "Coast/O";
		else if (sm_IN == sm_positive)
			return "Coast/+";
		else if (sm_IN == sm_negative)
			return "Coast/-";
		else
			return "Coast/? ";
	}
	
	
}
const char* motorStatusString(struct smartMotorStruct* smartMotorStructPtrIn)
{
	if (smartMotorStructPtrIn->Connected == ui8TRUE)
	{
		if (smartMotorStructPtrIn->StatusOK == ui8TRUE)
			return "Status OK     ";
		else
		{
			if (smartMotorStructPtrIn->OverCurrent == ui8TRUE)
				return "Over Current!";
			else if(smartMotorStructPtrIn->OverTemp == ui8TRUE)
				return "Over Temp!   ";
			else
				return "Status Alarm!";
		}
			
	}
	else
		return "NOT Connected!";
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

// The Help Menu String Generator
void writeUIHelpString(struct uiStruct* uiStructPtrIn)
{
//\\\\\ ccGripper - Help Menu \\\\\
//	Stop;         to Trigger Stop Move
//	UserLevel : Operator;
//				  to Promote User to Operator Level
//	Open;         to Trigger Open Move
//	Close;        to Trigger Close Move
//	Latch;        to Attempt Latch of High / Low Limits
//	AutoUpdate;   to Enable AutoUpdate of this Screen
//	Help;         to display This View
//	;             to Refresh Current View
//\\\\\ ccGripper - Key Pad Help
//	7  Home       8  Up
//	4  Back       5  Action    6  Forward
//	1  Main       2  Down

	if (uiStructPtrIn == nullptr)
	{
		return;
	}
	if (uiStructPtrIn->showHelp == ui8TRUE)
	{
		OPENSWITCH(uiStructPtrIn)
		case 0:
			PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), xstr(Mn), "Help", terminalSlashes()   END_MENU_LN;
		case 1:
			PRINT_MENU_LN  "\n%sStop; \tto Trigger Stop Move", cursorString(0)    END_MENU_LN;
		case 2: 
			PRINT_MENU_LN "\n\%sUserLevel:Operator; \n\t\tto Promote User to Operator Level", cursorString(0) END_MENU_LN;
		case 3:
			PRINT_MENU_LN  "\n%sOpen; \tto Trigger Open Move", cursorString(0)   END_MENU_LN;
		case 4:
			PRINT_MENU_LN  "\n%sClose; \tto Trigger Close Move", cursorString(0)    END_MENU_LN;
		case 5:
			PRINT_MENU_LN  "\n%sLatch; \tto Attempt Latch of High/Low Limits", cursorString(0)    END_MENU_LN;
		case 6:
			PRINT_MENU_LN  "\n%sAutoUpdate; \tto Enable AutoUpdate of this Screen", cursorString(0)    END_MENU_LN;
		
		case 7:
			PRINT_MENU_LN  "\n%sHelp; \tto display This View", cursorString(0)    END_MENU_LN;
		case 8:
			PRINT_MENU_LN  "\n%s; \t\tto Refresh Current View", cursorString(0)    END_MENU_LN;
		case 9:
			PRINT_MENU_LN  "\n%s %s - %s ", terminalSlashes(), xstr(Mn), "Key Pad Help"   END_MENU_LN;
		case 10:
			PRINT_MENU_LN  "\n%s7  Home\t8  Up", cursorString(0)    END_MENU_LN;
		case 11:
			PRINT_MENU_LN  "\n%s4  Back\t5  Action    6  Forward", cursorString(0)   END_MENU_LN;
		case 12:
			PRINT_MENU_LN  "\n%s1  Main\t2  Down", cursorString(0)    END_MENU_LN;
		default:
			uiStructPtrIn->showHelp = ui8FALSE;
		CLOSESWITCH(uiStructPtrIn)

	}
	else
		uiStructPtrIn->lines2print = 0;
}

// Printing
#define HISTORYINDEX(DISPLEN, LOGINDEX, LOGMAXSIZE) { \
	for (i = 1; i <= DISPLEN; i++) \
	{ \
		jj[i - 1] = LOGINDEX - (i-1)-1; \
		if (jj[i - 1] < 0) \
			jj[i - 1] = LOGMAXSIZE + jj[i - 1]; \
	} \
}
static int motorIndex = -1;
void printGripperFWConsoleAPI(struct ccGripperStruct* gripperStructPtrIn, struct uiStruct* uiStructPtrIn)
{
//Intelligent Gripper FW v0.1.0 - dev 26OCT2022
//	Gripper Status : uninitialized
//->Vacuum Enable Cmd : Off
//	Cylinder Extend Cmd : Retract
//	Vacuum Sensor ADC : 52428
//	Operations History(0 records, indecies 19 - 14) : 19 - 18 - 17 - 16 - 15 - 14 -
//	--      --              ----
//	PWM(Max, Nom, Delta)       Current(Max, Hold, Push, Nom)      TempMax         Vel(Max, Nom)    PosMinDelta StalledCycles OuterLimitDelta
//	--      --              ----
//	Motor Status     MotionRange LowStop  Start   Pos    Stop   HighStop LimitRange       VelFbk PWMCmd  CurFbk CurLimit Temp(CommTime Avg Max Min)
//	M0 : NOT Connected!0.00  999.99   0.00   0.00   0.00 - 999.99       0.00   |  |   0.00   0.0000    0.00   3.00   0.00   (0 0 0 1000 ms) Coast / X
//	M1 : NOT Connected!0.00  999.99   0.00   0.00   0.00 - 999.99       0.00   |  |   0.00   0.0000    0.00   3.00   0.00   (0 0 0 1000 ms) Coast / X
//	M2 : NOT Connected!0.00  999.99   0.00   0.00   0.00 - 999.99       0.00   |  |   0.00   0.0000    0.00   3.00   0.00   (0 0 0 1000 ms) Coast / X
//	M3 : NOT Connected!0.00  999.99   0.00   0.00   0.00 - 999.99       0.00   |  |   0.00   0.0000    0.00   3.00   0.00   (0 0 0 1000 ms) Coast / X
#define OpsHistDispLen (6)
	int jj[OpsHistDispLen] = {0};
	int i = 0;
	
	OPENSWITCH(uiStructPtrIn)
		case 0://Intelligent Gripper FW v0.1.0-dev 26OCT2022
			PRINT_MENU_LN "\nIntelligent Gripper FW v%s %s", ccGripper_VerString(), ccGripper_VerDateString()  END_MENU_LN;
		case 1://  Gripper Status:       uninitialized
			PRINT_MENU_LN "\n%sGripper Status:\t%s", cursorString(0), gripperModeString(gripperStructPtrIn->GripperMode[iCURRENT])   END_MENU_LN;
		case 2://->Vacuum Enable Cmd:    Off
			PRINT_MENU_LN "\n%sVacuum Enable Cmd:\t%s", cursorString(uiStructPtrIn->cursorIndex == 0), vacuumOnOffString(gripperStructPtrIn->VacuumExtend.VacEnableCmd)    END_MENU_LN;
		case 3://  Cylinder Extend Cmd : Retract
			PRINT_MENU_LN "\n%sCylinder Extend Cmd:\t%s", cursorString(uiStructPtrIn->cursorIndex == 1), cylinderExtendRetractString(gripperStructPtrIn->VacuumExtend.CyclinderExtendCmd)    END_MENU_LN;
		case 4://  Vacuum Sensor ADC:    52428
			PRINT_MENU_LN "\n%sVacuum Sensor ADC:\t%i", cursorString(0), gripperStructPtrIn->VacuumExtend.VacuumSensorADC END_MENU_LN;
		case 5://  Vacuum Sensor ADC:    52428
			PRINT_MENU_LN "\n%sPackets (bytes) Received / Sent:\t%i / %i\t(%i / %i)", cursorString(0), gripperStructPtrIn->PacketsAPI.inPackCounter, gripperStructPtrIn->PacketsAPI.outPackCounter, gripperStructPtrIn->PacketsAPI.inPackBytesCounter, gripperStructPtrIn->PacketsAPI.outPackBytesCounter END_MENU_LN;
		case 6://  Operations History (0 records, indecies 19-14): 19-  18-  17-  16-  15-  14-
			HISTORYINDEX(OpsHistDispLen, gripperStructPtrIn->OpsLogIndex, OPSLOGSIZE)
				PRINT_MENU_LN "\n%sOperations History (%i records, indecies %i-%i): %i-%s %i-%s %i-%s %i-%s %i-%s %i-%s\n\t--\t--\t\t----",
				cursorString(0),
				gripperStructPtrIn->OpsLogSize,
				jj[0],
				jj[5],
				jj[0],
				OpHistoryString(gripperStructPtrIn->OperationsLog[jj[0]]),
				jj[1],
				OpHistoryString(gripperStructPtrIn->OperationsLog[jj[1]]),
				jj[2],
				OpHistoryString(gripperStructPtrIn->OperationsLog[jj[2]]),
				jj[3],
				OpHistoryString(gripperStructPtrIn->OperationsLog[jj[3]]),
				jj[4],
				OpHistoryString(gripperStructPtrIn->OperationsLog[jj[4]]),
				jj[5],
				OpHistoryString(gripperStructPtrIn->OperationsLog[jj[5]]) END_MENU_LN;
			
		case 7: //        --      --              ----
			    //PWM(Max, Nom, Delta)       Current(Max, Hold, Push, Nom)      TempMax         Vel(Max, Nom)    PosMinDelta StalledCycles OuterLimitDelta
			PRINT_MENU_LN "\nPWM (Max, Nom, Delta) \t Current (Max, Hold, Push, Nom)  TempMax  Vel (Max,Nom)   PosMinDelta StalledCycles OuterLimitDelta" END_MENU_LN;
		case 8: 
			PRINT_MENU_LN "\n(%6.4f, %6.4f, %6.4f) (%5.2f, %5.2f, %5.2f, %5.2f)   %6.2f \t (%7.2f, %6.2f)    %6.2f     %i  \t\t %i\n\t--\t--\t\t----", 
				PWMMAX, 
				PWMNOMINAL, 
				PWMDELTA , 
				MOTORIMAX, 
				MOTORIHOLD, 
				MOTORIPUSH, 
				MOTORINOMINAL,
				MOTORMAXTEMP,
				ROTORVMAX,
				ROTORVNOMINAL,
				MOTORSTALLEDTHRESH,
				ROTORSTALLEDCYCLES,
				OUTERRANGEMULTIPLIER END_MENU_LN;


#define SM  (gripperStructPtrIn->SmartMotors[motorIndex])

		case 9://  Motor Status     MotionRange LowStop  Start   Pos    Stop   HighStop LimitRange       VelFbk PWMCmd  CurFbk CurLimit Temp  (CommTime Avg Max Min)
			PRINT_MENU_LN "\n%sMotor Status \t   MotionRange LowStop  Start   Pos    Stop   HighStop LimitRange ", cursorString(0) END_MENU_LN;
		case 10:
			PRINT_MENU_LN "\tVelFbk PWMCmd  CurFbk CurLimit Temp  (CommTime Avg Max Min)", cursorString(0) END_MENU_LN;

		case 11:	
		case 13:
		case 15:
		case 17://	M0: NOT Connected!0.00  999.99   0.00   0.00   0.00 - 999.99       0.00   |  |   0.00   0.0000    0.00   3.00   0.00   (0 0 0 1000 ms) Coast / X
			motorIndex++;
			if (motorIndex > 3)
				motorIndex = 0;
			PRINT_MENU_LN "\n%sM%i:\t%s\t%6.2f \t%6.2f %6.2f %6.2f %6.2f %6.2f \t%6.2f\t | ",
				cursorString(0),
				motorIndex,
				motorStatusString(&SM),
				SM.MotionRange,
				SM.RotorPositionLowStop,
				SM.RotorPositionFbkStart,
				SM.RotorPositionFbk,
				SM.RotorPositionFbkStop,
				SM.RotorPositionHighStop,
				SM.MotionRangeLimit END_MENU_LN;
		case 12:
		case 14:
		case 16:
		case 18:
			HISTORYINDEX(OpsHistDispLen, SM.DataIndex, MAXDATAENTRIES)
			PRINT_MENU_LN " | %6.2f %8.4f  %6.2f %6.2f %6.2f   (%i %i %i %i ms) %s",
				SM.RotorVelocityFbk,
				SM.MotorPWMCmd,
				SM.MotorCurrentFbk,
				SM.MotorCurrentLimitCMD,
				SM.MotorTempDegC,
				SM.millisThentoNow,
				SM.CommPeriodAverage,
				SM.CommPeriodAbsoluteMaximum,
				SM.CommPeriodAbsoluteMinimum,
				motionStateString(SM.MotionState, SM.MotorControlModeCMD) END_MENU_LN;
#undef SM(iii)
		
		default: uiStructPtrIn->currentMenuIndex = cM_devMOT0;
	CLOSESWITCH(uiStructPtrIn)

}
void printSmartMotorConsoleAPI(struct smartMotorStruct* smartMotorStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	//		--tStamp: 0     --              ----
	//	Motor0   tStamp | cPeriod  Triggers   State Conditions PWMCmd CurLimit CurFbk     VelFbk        PosFbk
	//	M0 : 0         0 | 0      0 0 0 0 1  Coast / X   0 1 1 0   0.0000     3.00     0.00     0.00     0.00
	//	M0 : 99         0 | 0      0 0 0 0 0  Coast / X   0 0 0 0   0.0000     0.00     0.00     0.00     0.00
	//	M0 : 98         0 | 0      0 0 0 0 0  Coast / X   0 0 0 0   0.0000     0.00     0.00     0.00     0.00
	//	M0 : 97         0 | 0      0 0 0 0 0  Coast / X   0 0 0 0   0.0000     0.00     0.00     0.00     0.00
	//	M0 : 96         0 | 0      0 0 0 0 0  Coast / X   0 0 0 0   0.0000     0.00     0.00     0.00     0.00
	//	M0 : 95         0 | 0      0 0 0 0 0  Coast / X   0 0 0 0   0.0000     0.00     0.00     0.00     0.00

	int jj[OpsHistDispLen] = { 0 };
	int i;
	static int k;
	OPENSWITCH(uiStructPtrIn)
		case 0://        --tStamp: 0     --              ----
			PRINT_MENU_LN "\n\t--tStamp: %i\t--\t\t----", getMillis() END_MENU_LN;
		case 1://  Motor0   tStamp|cPeriod  Triggers   State Conditions PWMCmd CurLimit CurFbk     VelFbk        PosFbk
			PRINT_MENU_LN "\n  Motor%i   tStamp|cPeriod  Triggers   State Conditions PWMCmd CurLimit CurFbk     VelFbk\tPosFbk", uiStructPtrIn->currentMenuIndex - cM_devMOT0 END_MENU_LN;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:// M0: 0         0 | 0      0 0 0 0 1  Coast / X   0 1 1 0   0.0000     3.00     0.00     0.00     0.00
			HISTORYINDEX(OpsHistDispLen, smartMotorStructPtrIn->DataIndex, MAXDATAENTRIES);
			PRINT_MENU_LN "\n  M%i:%2i  %8i |%i\t  %i %i %i %i %i  %s   %i %i %i %i %8.4f %8.2f %8.2f %8.2f %8.2f",
				uiStructPtrIn->currentMenuIndex - cM_devMOT0, 
				jj[uiStructPtrIn->linesprinted-2],
				smartMotorStructPtrIn->MilliStampData[jj[uiStructPtrIn->linesprinted - 2]],
				smartMotorStructPtrIn->CommPeriodAvgData[jj[uiStructPtrIn->linesprinted - 2]],
				((smartMotorStructPtrIn->TriggersData[jj[uiStructPtrIn->linesprinted - 2]]&0x10)>>4),
				((smartMotorStructPtrIn->TriggersData[jj[uiStructPtrIn->linesprinted - 2]]&0x08)>>3),
				((smartMotorStructPtrIn->TriggersData[jj[uiStructPtrIn->linesprinted - 2]]&0x04)>>2),
				((smartMotorStructPtrIn->TriggersData[jj[uiStructPtrIn->linesprinted - 2]]&0x02)>>1),
				((smartMotorStructPtrIn->TriggersData[jj[uiStructPtrIn->linesprinted - 2]]&0x01)),				
				motionStateString(smartMotorStructPtrIn->MotionStateData[jj[uiStructPtrIn->linesprinted - 2]], smartMotorStructPtrIn->MotorCtrlModeData[jj[uiStructPtrIn->linesprinted - 2]]),
				((smartMotorStructPtrIn->ConditionsData[jj[uiStructPtrIn->linesprinted - 2]] & 0x08) >> 3),
				((smartMotorStructPtrIn->ConditionsData[jj[uiStructPtrIn->linesprinted - 2]] & 0x04) >> 2),
				((smartMotorStructPtrIn->ConditionsData[jj[uiStructPtrIn->linesprinted - 2]] & 0x02) >> 1),
				((smartMotorStructPtrIn->ConditionsData[jj[uiStructPtrIn->linesprinted - 2]] & 0x01)),
				smartMotorStructPtrIn->MotorPWMCmdData[jj[uiStructPtrIn->linesprinted - 2]],
				smartMotorStructPtrIn->MotorCurrentLimitData[jj[uiStructPtrIn->linesprinted - 2]] ,
				smartMotorStructPtrIn->MotorCurrentFbkData[jj[uiStructPtrIn->linesprinted - 2]],
				smartMotorStructPtrIn->RotorVelocityFbkData[jj[uiStructPtrIn->linesprinted - 2]],
				smartMotorStructPtrIn->RotorPositionFbkData[jj[uiStructPtrIn->linesprinted - 2]] END_MENU_LN;


		default: if(++uiStructPtrIn->currentMenuIndex > cM_devMOT3)uiStructPtrIn->currentMenuIndex=cM_MainMenu;
	CLOSESWITCH(uiStructPtrIn)
}
void printSuctionExtendConsoleAPI(struct vacuumExtendStruct* vacuumExtendStructPtrIn, struct uiStruct* uiStructPtrIn)
{

}

// Parsing - Common
void parseUserLevel(struct uiStruct* uiStructPtrIn)
{

	OPENIF("UserLevel", uiStructPtrIn->currentMenuIndex)

		if (stringMatchCaseSensitive(&uiStructPtrIn->devptr->inbuff.charbuff[uiStructPtrIn->parseIndex], "Operator") == ui8TRUE)
			uiStructPtrIn->currentUserLevel = uiLevel_Operator;
		else if (stringMatchCaseSensitive(&uiStructPtrIn->devptr->inbuff.charbuff[uiStructPtrIn->parseIndex], "Observer") == ui8TRUE)
			uiStructPtrIn->currentUserLevel = uiLevel_Observer;

	CLOSEIF("UserLevel", uiStructPtrIn->currentMenuIndex)
}
void goParsedAction(enum currentMenuAction theCurrentAction, struct ccGripperStruct* gripperStructPtrIn, struct uiStruct* uiStructPtrIn)
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
void parseConsoleKeyPadAPI(struct ccGripperStruct* gripperStructPtrIn, struct uiStruct* uiStructPtrIn)
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
	goParsedAction(theCurrentAction, gripperStructPtrIn, uiStructPtrIn);

}

// Parsing - Modules
void parseGripperFWConsoleAPI(struct ccGripperStruct* gripperStructPtrIn, struct uiStruct* uiStructPtrIn)
{
	
	int i;

	if (stringMatchCaseSensitive(&uiStructPtrIn->devptr->inbuff.charbuff[uiStructPtrIn->parseIndex], "Open") == ui8TRUE)
	{
		gripperStructPtrIn->OperationsLog[gripperStructPtrIn->OpsLogIndex++] = 1;
		if (gripperStructPtrIn->OpsLogSize < OPSLOGSIZE)
			gripperStructPtrIn->OpsLogSize++;

		if (gripperStructPtrIn->OpsLogIndex >= OPSLOGSIZE)
			gripperStructPtrIn->OpsLogIndex = 0;

        gripperStructPtrIn->TriggerOpen = ui8TRUE;

	}


	if (stringMatchCaseSensitive(&uiStructPtrIn->devptr->inbuff.charbuff[uiStructPtrIn->parseIndex], "Close") == ui8TRUE)
	{
		gripperStructPtrIn->OperationsLog[gripperStructPtrIn->OpsLogIndex++] = 2;
		if (gripperStructPtrIn->OpsLogSize < OPSLOGSIZE)
			gripperStructPtrIn->OpsLogSize++;

		if (gripperStructPtrIn->OpsLogIndex >= OPSLOGSIZE)
			gripperStructPtrIn->OpsLogIndex = 0;

        gripperStructPtrIn->TriggerClose = ui8TRUE;
			
	}

	if (stringMatchCaseSensitive(&uiStructPtrIn->devptr->inbuff.charbuff[uiStructPtrIn->parseIndex], "Latch") == ui8TRUE)
	{
        gripperStructPtrIn->TriggerLatch = ui8TRUE;

	}
	if (stringMatchCaseSensitive(&uiStructPtrIn->devptr->inbuff.charbuff[uiStructPtrIn->parseIndex], "AutoUpdate") == ui8TRUE)
	{
		if(gripperStructPtrIn->AutoUpdate == ui8TRUE)
			gripperStructPtrIn->AutoUpdate = ui8FALSE;
		else
			gripperStructPtrIn->AutoUpdate = ui8TRUE;


	}
	if (stringMatchCaseSensitive(&uiStructPtrIn->devptr->inbuff.charbuff[uiStructPtrIn->parseIndex], "Push") == ui8TRUE)
	{
		gripperStructPtrIn->OperationsLog[gripperStructPtrIn->OpsLogIndex++] = 7;
		if (gripperStructPtrIn->OpsLogSize < OPSLOGSIZE)
			gripperStructPtrIn->OpsLogSize++;

		if (gripperStructPtrIn->OpsLogIndex >= OPSLOGSIZE)
			gripperStructPtrIn->OpsLogIndex = 0;

		for (i = 0; i < NUMMOTORS; i++)
		{
			gripperStructPtrIn->SmartMotors[i].TriggerPush = ui8TRUE;
		}
	}
	if (stringMatchCaseSensitive(&uiStructPtrIn->devptr->inbuff.charbuff[uiStructPtrIn->parseIndex], "Hold") == ui8TRUE)
	{
		gripperStructPtrIn->OperationsLog[gripperStructPtrIn->OpsLogIndex++] = 8;
		if (gripperStructPtrIn->OpsLogSize < OPSLOGSIZE)
			gripperStructPtrIn->OpsLogSize++;

		if (gripperStructPtrIn->OpsLogIndex >= OPSLOGSIZE)
			gripperStructPtrIn->OpsLogIndex = 0;

		for (i = 0; i < NUMMOTORS; i++)
		{
			gripperStructPtrIn->SmartMotors[i].TriggerHold = ui8TRUE;
		}
	}
	OPENIF("PWM", uiStructPtrIn->currentMenuIndex)

		float floatIn;
		int q;
		UI_8 holdIt = 0xff;
		for (q = 0; q < charBuffMax; q++)
		{
			if (uiStructPtrIn->devptr->inbuff.charbuff[q] == ';' )
			{
				holdIt = uiStructPtrIn->devptr->inbuff.charbuff[q];
				uiStructPtrIn->devptr->inbuff.charbuff[q] = 0x00;
				break;
			}
		}

		

		if (ATO_F(&uiStructPtrIn->devptr->inbuff.charbuff[uiStructPtrIn->parseIndex], &floatIn))
		{
			if (floatIn < PWMNOMINAL && floatIn > -PWMNOMINAL)
			{
				if(floatIn > 0.0)
					gripperStructPtrIn->OperationsLog[gripperStructPtrIn->OpsLogIndex++] = 4;
				else if(floatIn < 0.0)
					gripperStructPtrIn->OperationsLog[gripperStructPtrIn->OpsLogIndex++] = 5;
				else
					gripperStructPtrIn->OperationsLog[gripperStructPtrIn->OpsLogIndex++] = 6;

				if (gripperStructPtrIn->OpsLogSize < OPSLOGSIZE)
					gripperStructPtrIn->OpsLogSize++;

				if (gripperStructPtrIn->OpsLogIndex >= OPSLOGSIZE)
					gripperStructPtrIn->OpsLogIndex = 0;

				for (i = 0; i < NUMMOTORS; i++)
				{
					gripperStructPtrIn->SmartMotors[i].MotorControlModeCMD = bldc_spinpwm;
					gripperStructPtrIn->SmartMotors[i].MotorPWMCmd = floatIn;
				}

			}
		}
		uiStructPtrIn->devptr->inbuff.charbuff[q] = holdIt;

	CLOSEIF("PWM", uiStructPtrIn->currentMenuIndex)
	
	
}
void parseSmartMotorConsoleAPI(struct smartMotorStruct* smartMotorStructPtrIn, struct uiStruct* uiStructPtrIn)
{

}
void parseSuctionExtendConsoleAPI(struct vacuumExtendStruct* vacuumExtendStructPtrIn, struct uiStruct* uiStructPtrIn)
{

}
