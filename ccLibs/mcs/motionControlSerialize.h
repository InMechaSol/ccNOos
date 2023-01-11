/** \file motionControl.h
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

#ifndef __ccNOos_MCS_SER__
#define __ccNOos_MCS_SER__

#include "packets_api.h"

enum mcsSPDSelector
{
    mcsNone = 0,
    mcsPlndT,
    mcsPlnTalpha,
    mcsPlnTomega,
    mcsPlnTMotion,
    mcsPlnDeltaPos,
    mcsDesiredPos,
    mcsDesiredVel,
    mcsDesiredAcc,
    mcsActMotionState,
    mcsDesiredMotionState,
    mcsUseEstimatedVel,
    mcsEstimatedVelocity,
    mcsCommandVelocity,
    mcsLastPosition,
    mcsPositionResolution,
    mcsActualControlMode,
    mcsDesiredControlMode,
    mcsPosCMD,
    mcsPosFBK,
    mcsPosERR,
    mcsPosPLim,
    mcsPosNLim,
    mcsPosCtrldT,
    mcsPosCtrlKv,
    mcsPosCtrlcmdLast,
    mcsPosCtrlcmdVel,
    mcsVelCMD,
    mcsVelFBK,
    mcsVelERR,
    mcsVelPLim,
    mcsVelNLim,
    mcsVelCtrldT,
    mcsVelCtrlErr,
    mcsVelCtrlErrLast,
    mcsVelCtrlOut,
    mcsVelCtrlOutLast,
    mcsVelCtrlOutI,
    mcsVelCtrlOutP,
    mcsVelCtrlKp,
    mcsVelCtrlTi,
    mcsVelCtrlAlpha,
    mcsVelCtrlReset,
    mcsVelCtrlSaturated,
    mcsCurCMD,
    mcsCurFBK,
    mcsCurERR,
    mcsCurPLim,
    mcsCurNLim,
    mcsMotConTorque,
    mcsMotVcc,
    mcsMotdT,
    mcsMotTime,
    mcsMotReset,
    mcsMotWMinRes,
    mcsMotIdot,
    mcsMotI,
    mcsMotWdot,
    mcsMotW,
    mcsMotJ,
    mcsMotR,
    mcsMotL,
    mcsMotKm,
    mcsMotV,
    mcsMotVemf,
    mcsMotTorque,
    mcsMotFricTor,
    mcsMotEff,
    mcsCurCtrldT,
    mcsCurCtrlErr,
    mcsCurCtrlErrLast,
    mcsCurCtrlOut,
    mcsCurCtrlOutLast,
    mcsCurCtrlOutI,
    mcsCurCtrlOutP,
    mcsCurCtrlKp,
    mcsCurCtrlTi,
    mcsCurCtrlAlpha,
    mcsCurCtrlReset,
    mcsCurCtrlSaturated,
    mcsPWMCMD,
    mcsEND
};

// Forward Declarations
struct axisStruct;
struct SPDStruct;
// SPD Array Population Function
void LinkAxisSPDStructArray(struct axisStruct* mystruct, struct SPDStruct* myArray);


#ifdef __cplusplus

// Forward Declaration
class SmartMotorDevice;

// The Axis SPD Class
class AxisSPD: public SPDClass
{
private:
    SmartMotorDevice* smDevPtr = nullptr;
    enum mcsSPDSelector AxisVarSelection = mcsNone;
public:
    AxisSPD(enum mcsSPDSelector AxisVarSelectionIn, SmartMotorDevice* smDevPtrIn);
    enum mcsSPDSelector getSPDSelector();
    SmartMotorDevice* getSMDevPtr();
    float getFloatVal();
    const char* getLabelString();
    const char* getUnitsString();
};

#endif // !__cplusplus
#endif // !__ccNOos_MCS_SER__
