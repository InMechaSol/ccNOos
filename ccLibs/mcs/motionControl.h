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

#ifndef __ccNOos_MCS__
#define __ccNOos_MCS__

#include "packets_api.h"

///
/// \brief The controlTypeEnum enum
///
enum controlTypeEnum
{
    controlPosition,
    controlVelocity,
    controlCurrent,
    controlPWM
};
///
/// \brief The motionStateEnum enum
///
enum motionStateEnum
{
    motionStopped,
    motionPositive,
    motionNegative
};

///
/// \brief The planningStruct class
///
struct planningStruct
{
    float dT, delataPos, Talpha, Tomega, Tmotion;
    float desiredPos, motionVelocity, motionAcceleration;
    float estVelocity, cmdVelocity, LastFbkPosition, PositionResolution;
    enum motionStateEnum actualMotionState;
    enum motionStateEnum desiredMotionState;
    enum controlTypeEnum actualControlMode;
    enum controlTypeEnum desiredControlMode;
    UI_8 useEstimatedVelocity;
};
struct planningStruct createplanningStruct();
float sqrtFloat(float inFloat);

///
/// \brief The posControlStruct class
///
struct posControlStruct
{
    float dT;
    float Kv;
    float cmdLast, cmdVel;
};
struct posControlStruct createposConrolStruct();

///
/// \brief The controledFloat class
///
struct controledFloat
{
    float Cmd;
    float Fbk;
    float Err;
    float LimitPos;
    float LimitNeg;
};
struct controledFloat createcontroledFloat();

///
/// \brief The piControllerStruct class
///
struct piControllerStruct
{
    float dT;
    float xn1, xn;
    float yn1, yn, yin, ypn;
    float Kp, Ti, alpha;
    UI_8 reset, saturated;
};
struct piControllerStruct createpiControllerStruct();
void preparepiControllerStruct(struct piControllerStruct* piControllerPtr);
void executepiControllerStruct(struct piControllerStruct* piControllerPtr);

///
/// \brief The dcMotorStruct class
///
struct dcMotorStruct
{
    float ContinousTorque, Vcc;
    float Time, dT, WminResolution;
    float Idot, I, Wdot, J, W, R, L, Km, V, Vemf, Torque, FrictionTorque, Efficiency;
    float a11, a12, a21, a22;
    UI_8 Reset;
};
struct dcMotorStruct createdcMotorStruct();
void preparedcMotorStruct(struct dcMotorStruct* dcMotorStructPtrIn);
void executedcMotorStruct(struct dcMotorStruct* dcMotorStructPtrIn);



///
/// \brief The axisStruct class
///
struct axisStruct
{
    struct planningStruct Planning;
    struct controledFloat Position;
    struct posControlStruct PosController;
    struct controledFloat Velocity;
    struct piControllerStruct VelController;
    struct controledFloat Current;
    float torqueCmd, voltageCmd, voltageLimit;
    struct dcMotorStruct MotorModel;
    struct piControllerStruct CurController;
    float PWMCmd, PWMLimit;
    UI_8 ctrlEnabled, currentCtrlEnabled;
    UI_8 PWMSaturated, CurrentSaturated;
};
struct axisStruct createaxisStruct();

///
/// \brief Cascade Control Loops
///
///
void planningLoop(struct axisStruct* axisStructPtr);
void positionLoop(struct axisStruct* axisStructPtr);
void velocityLoop(struct axisStruct* axisStructPtr);
void currentLoop(struct axisStruct* axisStructPtr);

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
void LinkAxisSPDStructArray(struct axisStruct* mystruct, struct SPDStruct* myArray);


#ifdef __cplusplus

class SmartMotorDevice
{
private:

struct axisStruct* mystruct;
struct SPDStruct AxisSPDStructArray[mcsEND];

protected:
    virtual void readSMData(){return;}
    virtual void writeSMData(){return;}

public:
    SmartMotorDevice(struct axisStruct* mystructIn);
    void prepare();
    void execute();
    struct SPDStruct* getSPDArray();
};






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



#endif
#endif
