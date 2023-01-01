/** \file SmartMotorsDEV.h
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
#ifndef __SMARTMOTORMOD__
#define __SMARTMOTORMOD__  

#include "console_menu.h"

///////////////////////////////////////////////////////////////////
// 
// Here the smart motor exists in the context of a gripper finger
// actuator.  It must simply open/close reliably, without fail.
// Positive Motion, Negative Motion, Stopping, Holding, are all that
// is required.
// 
// Initial state of an individual gripper finger is unknown and must
// first be determined.  Characterization based on PWM cmd and Current
// limits can be conducted using the algorithm constants below.
// 
// Determination of state:
//  Concept of Configured - A finger smart motor is only configured if its range is known
//  Concept of Range - Bound by variable outer limits, known range measured from feedback
//  Concept of Grip (close) - move at nominal force toward close direction, 
//      increase force on stall, hold with force
//  Concept of Grip (open) - move at nominal force toward open direction, stop force on cause
//      Causes of Stop Force:
//          1)  
// 
// Current Limit Control:
//   MotorINominal - is the initial current limit for any motion
//   MotorIPush - is an mid-range current limit applied when stalled
//   MotorIHold - is the highest current limit applied during intentional hold
//
// Thermal/Force Protections:
//   Set motor to coast mode on detection of
//      1) Over Current
//      2) Over Temperature
//      3) Outerlimts of Range
// 
// System Characterization:
//  1) Determine Minimum PWM in each direction that creates motion
//  2) Determine Variable Outer Limits on Range
//  3) Determine Avg Speed over range vs PWM 
//
//
///////////////////////////////////////////////////////////////////
#define LIMITSINITVAL (0.0)  // radians - unknown outer limits of range
#define MOTORMAXTEMP 70.0       // deg c - Triggers OverTemp!

#define MOTORINOMINAL 1.5       // Amps
#define MOTORIPUSH 3.0          // Amps
#define MOTORIHOLD 5.0          // Amps
#define MOTORIMAX 20.0           // Amps - Triggers OverCurrent!

#define ROTORVMAX 140.0          // Rad/sec
#define ROTORVNOMINAL 30.0      // Rad/sec

#define PWMMAX 0.09             // 5% of full throttle
#define PWMNOMINAL 0.03         // 2% of full throttle
#define PWMDELTA 0.002          // .2% of full throttle

#define MOTORSTALLEDTHRESH 0.01 // Radians
#define ROTORSTALLEDCYCLES (12) // loop execution cycles ~120ms on teensy4.1
#define OUTERRANGEMULTIPLIER (3)// 3 x the Motors Stalled Threshold

///////////////////////////////////
// this group is based on the api
// from the iq 23-06 smart motor
#define MOTORCTRLMODEPWM (2)
#define MOTORCTRLMODECOAST (5)
enum brshDriveCtrlMode
{
    bldc_unknown = 0,
    bldc_coast = 5,
    bldc_spinpwm = 2
};
//
// END - the iq 23-06 stuff
///////////////////////////////////

enum enumSmartMotorState
{
    sm_stopped,
    sm_stalled,
    sm_positive,
    sm_negative
};

// Module Data Structure
struct smartMotorStruct
{
    struct devicedatastruct* devptr;    
    UI_32 millisNow, millisThen, millisThentoNow;
    UI_8 StatusOK, StatusOKLast;

    UI_8 OverTemp;
    UI_8 OverCurrent;

    UI_8 Connected, ConnectedLast;

    UI_8 MotorsCommPhase;
    UI_8 MotorsCommSubPhase;

    enum brshDriveCtrlMode MotorControlModeCMD;
    enum brshDriveCtrlMode MotorControlModeFbk;
    UI_8 ModuleID;

    UI_8 TriggerPositiveMove;
    UI_8 TriggerNegativeMove;
    UI_8 TriggerStop;

    UI_8 TriggerPush;
    UI_8 TriggerHold;
    
    UI_8 StalledCyclesCounter;
    enum enumSmartMotorState MotionState;
    enum enumSmartMotorState MotionStateLast;

    float RotorPositionFbk, RotorPositionFbkStart, RotorPositionFbkStop;
    float MotionRange, MotionRangeLimit;
    float RotorPositionFbkLast;

    float RotorVelocityFbk;
    float MotorCurrentFbk;
    float MotorTempDegC;

    float MotorCurrentLimitCMD;
    float MotorCurrentLimitFBK;
    //float RotorVelocityCmd;

    float MotorPWMCmd, MotorPWMCmdReturned;

    float RotorPositionCmd;
    float RotorPositionHighStop;
    float RotorPositionLowStop;

#define PeriodAvgSamples (20)
    UI_32 CommPeriodAbsoluteMaximum;
    UI_32 CommPeriodAbsoluteMinimum;
    UI_32 CommPeriodAvgs[PeriodAvgSamples];
    UI_32 CommPeriodAccumulation;
    UI_32 CommPeriodAverage;
    UI_8 PeriodAvgSampleSize;
    UI_8 PeridoAvgSampleIndex;
    UI_8 LatchHighStopPos, LatchLowStopPos;

    // system characterization data
    #define MAXDATAENTRIES (100)
    int DataIndex;
    int DataSize, DataSizeLast;
    UI_8 TriggersData[MAXDATAENTRIES];
    UI_8 ConditionsData[MAXDATAENTRIES];

    float MotorPWMCmdData[MAXDATAENTRIES];
    enum enumSmartMotorState MotionStateData[MAXDATAENTRIES];
    enum brshDriveCtrlMode MotorCtrlModeData[MAXDATAENTRIES];
    float MotorCurrentFbkData[MAXDATAENTRIES];
    float MotorCurrentLimitData[MAXDATAENTRIES];
    float RotorVelocityFbkData[MAXDATAENTRIES];
    float RotorPositionFbkData[MAXDATAENTRIES];
    float MotorTempData[MAXDATAENTRIES];
    
    UI_32 MilliStampData[MAXDATAENTRIES];
    UI_32 CommPeriodAvgData[MAXDATAENTRIES];
};
struct smartMotorStruct createsmartMotorStruct();
UI_32 getMillis();

void captureDataLogSamples(struct smartMotorStruct* smartMotorStructPtrIn);
void clearDataLogSamples(struct smartMotorStruct* smartMotorStructPtrIn);

void smartMotorLoop(struct smartMotorStruct* smartMotorStructPtrIn);

void readMotorData(struct smartMotorStruct* smartMotorStructPtrIn);
void writeMotorData(struct smartMotorStruct* smartMotorStructPtrIn);
void parseMotorData(struct smartMotorStruct* smartMotorStructPtrIn);
void packageMotorData(struct smartMotorStruct* smartMotorStructPtrIn);

#ifdef __USINGCONSOLEMENU

void parseSmartMotorConsoleAPI(struct smartMotorStruct* smartMotorStructPtrIn, struct uiStruct* uiStructPtrIn);
void printSmartMotorConsoleAPI(struct smartMotorStruct* smartMotorStructPtrIn, struct uiStruct* uiStructPtrIn);

#endif

#ifdef __cplusplus


#endif // !__cplusplus
#endif // !__SMARTMOTORMOD__