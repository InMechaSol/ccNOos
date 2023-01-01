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

#include "version_config.h"

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



#ifdef __cplusplus

class SmartMotorDevice
{
private:

struct axisStruct mystruct = createaxisStruct();
struct SPDStruct AxisSPDStructArray[mcsEND] ={
                {mcsNone,                   "",                     "",                 1,NULL_TYPE,    0,                                              nullptr},
                {mcsPlndT,                  "dT",                   "(sec)",            1,FLOAT_TYPE,   sizeof(mystruct.Planning.dT),                   &mystruct.Planning.dT},
                {mcsPlnTalpha,              "Talpha",               "(sec)",            1,FLOAT_TYPE,   sizeof(mystruct.Planning.Talpha),               &mystruct.Planning.Talpha},
                {mcsPlnTomega,              "Tomega",               "(sec)",            1,FLOAT_TYPE,   sizeof(mystruct.Planning.Tomega),               &mystruct.Planning.Tomega},
                {mcsPlnTMotion,             "Tmotion",              "(sec)",            1,FLOAT_TYPE,   sizeof(mystruct.Planning.Tmotion),              &mystruct.Planning.Tmotion},
                {mcsPlnDeltaPos,            "Delta Position",       "(rad)",            1,FLOAT_TYPE,   sizeof(mystruct.Planning.delataPos),            &mystruct.Planning.delataPos},
                {mcsDesiredPos,             "Desired Position",     "(rad)",            0,FLOAT_TYPE,   sizeof(mystruct.Planning.desiredPos),           &mystruct.Planning.desiredPos},
                {mcsDesiredVel,             "Desired Velocity",     "(rad/sec)",        0,FLOAT_TYPE,   sizeof(mystruct.Planning.motionVelocity),       &mystruct.Planning.motionVelocity},
                {mcsDesiredAcc,             "Desired Acceleration", "(rad/sec/sec)",    0,FLOAT_TYPE,   sizeof(mystruct.Planning.motionAcceleration),   &mystruct.Planning.motionAcceleration},
                {mcsActMotionState,         "Actual Motion State",  "",                 1,SIGNED_TYPE,  sizeof(mystruct.Planning.actualMotionState),    &mystruct.Planning.actualMotionState},
                {mcsDesiredMotionState,     "Desired Motion State", "",                 1,SIGNED_TYPE,  sizeof(mystruct.Planning.desiredMotionState),   &mystruct.Planning.desiredMotionState},
                {mcsUseEstimatedVel,        "Use Est. Velocity",    "(yes/no)",         0,UNSIGNED_TYPE,sizeof(mystruct.Planning.actualControlMode),    &mystruct.Planning.actualControlMode},
                {mcsEstimatedVelocity,      "Estimated Velocity",   "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct.Planning.estVelocity),          &mystruct.Planning.estVelocity},
                {mcsCommandVelocity,        "Command Velocity",     "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct.Planning.cmdVelocity),          &mystruct.Planning.cmdVelocity},
                {mcsLastPosition,           "Last Position",        "(rad)",            1,FLOAT_TYPE,   sizeof(mystruct.Planning.LastFbkPosition),      &mystruct.Planning.LastFbkPosition},
                {mcsPositionResolution,     "Position Resolution",  "(rad)",            0,FLOAT_TYPE,   sizeof(mystruct.Planning.PositionResolution),   &mystruct.Planning.PositionResolution},
                {mcsActualControlMode,      "Actual Control Mode",  "",                 1,SIGNED_TYPE,  sizeof(mystruct.Planning.actualControlMode),    &mystruct.Planning.actualControlMode},
                {mcsDesiredControlMode,     "Desired Control Mode", "",                 0,SIGNED_TYPE,  sizeof(mystruct.Planning.desiredControlMode),   &mystruct.Planning.desiredControlMode},
                {mcsPosCMD,                 "Position CMD",         "(rad)",            0,FLOAT_TYPE,   sizeof(mystruct.Position.Cmd),                  &mystruct.Position.Cmd},
                {mcsPosFBK,                 "Position FBK",         "(rad)",            1,FLOAT_TYPE,   sizeof(mystruct.Position.Fbk),                  &mystruct.Position.Fbk},
                {mcsPosFBK,                 "Position ERR",         "(rad)",            1,FLOAT_TYPE,   sizeof(mystruct.Position.Err),                  &mystruct.Position.Err},
                {mcsPosPLim,                "Position +Limit",      "(rad)",            0,FLOAT_TYPE,   sizeof(mystruct.Position.LimitPos),             &mystruct.Position.LimitPos},
                {mcsPosNLim,                "Position -Limit",      "(rad)",            0,FLOAT_TYPE,   sizeof(mystruct.Position.LimitNeg),             &mystruct.Position.LimitNeg},
                {mcsPosCtrldT,              "PosLoop dT",           "(sec)",            1,FLOAT_TYPE,   sizeof(mystruct.PosController.dT),              &mystruct.PosController.dT},
                {mcsPosCtrlKv,              "PosLoop Kv",           "(1/sec)",          0,FLOAT_TYPE,   sizeof(mystruct.PosController.Kv),              &mystruct.PosController.Kv},
                {mcsPosCtrlcmdLast,         "PosLoop CMDLast",      "(rad)",            1,FLOAT_TYPE,   sizeof(mystruct.PosController.cmdLast),         &mystruct.PosController.cmdLast},
                {mcsPosCtrlcmdVel,          "PosLoop CMDVel",       "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct.PosController.cmdVel),          &mystruct.PosController.cmdVel},
                {mcsVelCMD,                 "Velocity CMD",         "(rad/sec)",        0,FLOAT_TYPE,   sizeof(mystruct.Velocity.Cmd),                  &mystruct.Velocity.Cmd},
                {mcsVelFBK,                 "Velocity FBK",         "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct.Velocity.Fbk),                  &mystruct.Velocity.Fbk},
                {mcsVelERR,                 "Velocity ERR",         "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct.Velocity.Err),                  &mystruct.Velocity.Err},
                {mcsVelPLim,                "Velocity +Limit",      "(rad/sec)",        0,FLOAT_TYPE,   sizeof(mystruct.Velocity.LimitPos),             &mystruct.Velocity.LimitPos},
                {mcsVelNLim,                "Velocity -Limit",      "(rad/sec)",        0,FLOAT_TYPE,   sizeof(mystruct.Velocity.LimitNeg),             &mystruct.Velocity.LimitNeg},
                {mcsVelCtrldT,              "VelLoop dT",           "(sec)",            1,FLOAT_TYPE,   sizeof(mystruct.VelController.dT),              &mystruct.VelController.dT},
                {mcsVelCtrlErr,             "VelLoop Error",        "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct.VelController.xn),              &mystruct.VelController.xn},
                {mcsVelCtrlErrLast,         "VelLoop ErrorLast",    "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct.VelController.xn1),             &mystruct.VelController.xn1},
                {mcsVelCtrlOut,             "VelLoop Torque Cmd",   "(Nm)",             0,FLOAT_TYPE,   sizeof(mystruct.VelController.yn),              &mystruct.VelController.yn},
                {mcsVelCtrlOutLast,         "VelLoop TorCmd Last",  "(Nm)",             1,FLOAT_TYPE,   sizeof(mystruct.VelController.yn1),             &mystruct.VelController.yn1},
                {mcsVelCtrlOutI,            "VelLoop TorCmd I",     "(Nm)",             1,FLOAT_TYPE,   sizeof(mystruct.VelController.yin),             &mystruct.VelController.yin},
                {mcsVelCtrlOutP,            "VelLoop TorCmd P",     "(Nm)",             1,FLOAT_TYPE,   sizeof(mystruct.VelController.ypn),             &mystruct.VelController.ypn},
                {mcsVelCtrlKp,              "VelLoop Kp",           "(Nm*sec/rad)",     0,FLOAT_TYPE,   sizeof(mystruct.VelController.Kp),              &mystruct.VelController.Kp},
                {mcsVelCtrlTi,              "VelLoop Ti",           "(sec)",            0,FLOAT_TYPE,   sizeof(mystruct.VelController.Ti),              &mystruct.VelController.Ti},
                {mcsVelCtrlAlpha,           "VelLoop Alpha",        "(Nm*sec/rad)",     1,FLOAT_TYPE,   sizeof(mystruct.VelController.alpha),           &mystruct.VelController.alpha},
                {mcsVelCtrlReset,           "VelLoop Reset",        "(yes/no)",         0,UNSIGNED_TYPE,sizeof(mystruct.VelController.reset),           &mystruct.VelController.reset},
                {mcsVelCtrlSaturated,       "VelLoop Saturated",    "(yes/no)",         1,UNSIGNED_TYPE,sizeof(mystruct.VelController.saturated),       &mystruct.VelController.saturated},
                {mcsCurCMD,                 "Current CMD",          "(A)",              0,FLOAT_TYPE,   sizeof(mystruct.Current.Cmd),                   &mystruct.Current.Cmd},
                {mcsCurFBK,                 "Current FBK",          "(A)",              1,FLOAT_TYPE,   sizeof(mystruct.Current.Fbk),                   &mystruct.Current.Fbk},
                {mcsCurERR,                 "Current ERR",          "(A)",              1,FLOAT_TYPE,   sizeof(mystruct.Current.Err),                   &mystruct.Current.Err},
                {mcsCurPLim,                "Current +Limit",       "(A)",              0,FLOAT_TYPE,   sizeof(mystruct.Current.LimitPos),              &mystruct.Current.LimitPos},
                {mcsCurNLim,                "Current -Limit",       "(A)",              0,FLOAT_TYPE,   sizeof(mystruct.Current.LimitNeg),              &mystruct.Current.LimitNeg},
                {mcsMotConTorque,           "Motor Cont. Torque",   "(Nm)",             0,FLOAT_TYPE,   sizeof(mystruct.MotorModel.ContinousTorque),    &mystruct.MotorModel.ContinousTorque},
                {mcsMotVcc,                 "Motor Vcc",            "(V)",              0,FLOAT_TYPE,   sizeof(mystruct.MotorModel.Vcc),                &mystruct.MotorModel.Vcc},
                {mcsMotdT,                  "Motor dT",             "(sec)",            1,FLOAT_TYPE,   sizeof(mystruct.MotorModel.dT),                 &mystruct.MotorModel.dT},
                {mcsMotTime,                "Motor Real Time",      "(sec)",            1,FLOAT_TYPE,   sizeof(mystruct.MotorModel.Time),               &mystruct.MotorModel.Time},
                {mcsMotReset,               "Motor Reset",          "(yes/no)",         0,UNSIGNED_TYPE,sizeof(mystruct.MotorModel.Reset),              &mystruct.MotorModel.Reset},
                {mcsMotWMinRes,             "Motor Vel. Res.",      "(rad/sec)",        0,FLOAT_TYPE,   sizeof(mystruct.MotorModel.WminResolution),     &mystruct.MotorModel.WminResolution},
                {mcsMotIdot,                "Motor Cur. Delta",     "(A/sec)",          1,FLOAT_TYPE,   sizeof(mystruct.MotorModel.Idot),               &mystruct.MotorModel.Idot},
                {mcsMotI,                   "Motor Current",        "(A)",              1,FLOAT_TYPE,   sizeof(mystruct.MotorModel.I),                  &mystruct.MotorModel.I},
                {mcsMotWdot,                "Motor Rotor Acc.",     "(rad/sec/sec)",    1,FLOAT_TYPE,   sizeof(mystruct.MotorModel.Wdot),               &mystruct.MotorModel.Wdot},
                {mcsMotW,                   "Motor Rotor Vel.",     "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct.MotorModel.W),                  &mystruct.MotorModel.W},
                {mcsMotJ,                   "Motor Rotor J",        "(Kg*m*m)",         0,FLOAT_TYPE,   sizeof(mystruct.MotorModel.J),                  &mystruct.MotorModel.J},
                {mcsMotR,                   "Motor Rotor R",        "(Ohms)",           0,FLOAT_TYPE,   sizeof(mystruct.MotorModel.R),                  &mystruct.MotorModel.R},
                {mcsMotL,                   "Motor Rotor Inductance","(Henrys)",        0,FLOAT_TYPE,   sizeof(mystruct.MotorModel.L),                  &mystruct.MotorModel.L},
                {mcsMotKm,                  "Motor Constant",       "(A/Nm)",           0,FLOAT_TYPE,   sizeof(mystruct.MotorModel.Km),                 &mystruct.MotorModel.Km},
                {mcsMotV,                   "Motor Voltage",        "(V)",              0,FLOAT_TYPE,   sizeof(mystruct.MotorModel.V),                  &mystruct.MotorModel.V},
                {mcsMotVemf,                "Motor Back EMF",       "(V)",              1,FLOAT_TYPE,   sizeof(mystruct.MotorModel.Vemf),               &mystruct.MotorModel.Vemf},
                {mcsMotTorque,              "Motor Rotor Torque",   "(Nm)",             1,FLOAT_TYPE,   sizeof(mystruct.MotorModel.Torque),             &mystruct.MotorModel.Torque},
                {mcsMotFricTor,             "Motor Rotor Friction", "(Nm)",             1,FLOAT_TYPE,   sizeof(mystruct.MotorModel.FrictionTorque),     &mystruct.MotorModel.FrictionTorque},
                {mcsMotEff,                 "Motor Efficiency",     "(0 to 1)",         0,FLOAT_TYPE,   sizeof(mystruct.MotorModel.Efficiency),         &mystruct.MotorModel.Efficiency},
                {mcsCurCtrldT,              "CurLoop dT",           "(sec)",            1,FLOAT_TYPE,   sizeof(mystruct.CurController.dT),              &mystruct.CurController.dT},
                {mcsCurCtrlErr,             "CurLoop Error",        "(A)",              1,FLOAT_TYPE,   sizeof(mystruct.CurController.xn),              &mystruct.CurController.xn},
                {mcsCurCtrlErrLast,         "CurLoop ErrorLast",    "(A)",              1,FLOAT_TYPE,   sizeof(mystruct.CurController.xn1),             &mystruct.CurController.xn1},
                {mcsCurCtrlOut,             "CurLoop Voltage Cmd",  "(V)",              1,FLOAT_TYPE,   sizeof(mystruct.CurController.yn),              &mystruct.CurController.yn},
                {mcsCurCtrlOutLast,         "CurLoop VoltsCmd Last","(V)",              1,FLOAT_TYPE,   sizeof(mystruct.CurController.yn1),             &mystruct.CurController.yn1},
                {mcsCurCtrlOutI,            "CurLoop VoltsCmd I",   "(V)",              1,FLOAT_TYPE,   sizeof(mystruct.CurController.yin),             &mystruct.CurController.yin},
                {mcsCurCtrlOutP,            "CurLoop VoltsCmd P",   "(V)",              1,FLOAT_TYPE,   sizeof(mystruct.CurController.ypn),             &mystruct.CurController.ypn},
                {mcsCurCtrlKp,              "CurLoop Kp",           "(V/A)",            0,FLOAT_TYPE,   sizeof(mystruct.CurController.Kp),              &mystruct.CurController.Kp},
                {mcsCurCtrlTi,              "CurLoop Ti",           "(sec)",            0,FLOAT_TYPE,   sizeof(mystruct.CurController.Ti),              &mystruct.CurController.Ti},
                {mcsCurCtrlAlpha,           "CurLoop Alpha",        "(V/A)",            1,FLOAT_TYPE,   sizeof(mystruct.CurController.alpha),           &mystruct.CurController.alpha},
                {mcsCurCtrlReset,           "CurLoop Reset",        "(yes/no)",         0,UNSIGNED_TYPE,sizeof(mystruct.CurController.reset),           &mystruct.CurController.reset},
                {mcsCurCtrlSaturated,       "CurLoop Saturated",    "(yes/no)",         0,UNSIGNED_TYPE,sizeof(mystruct.CurController.saturated),       &mystruct.CurController.saturated},
                {mcsPWMCMD,                 "PWM Command",          "(-1 to 1)",        0,UNSIGNED_TYPE,sizeof(mystruct.CurController.reset),           &mystruct.CurController.reset},
                                            };

protected:
    virtual void readSMData(){return;}
    virtual void writeSMData(){return;}

public:
    SmartMotorDevice();
    void prepare();
    void execute();
    struct SPDStruct* getSPDArray();
};


// make it a base class instead
class SPDClass
{
private:
    void* DataPtr;
    int VarIndex;
    struct SPDStruct* SPDArray;

public:
    SPDClass(int VarIndexIn, void* DataPtrIn, struct SPDStruct* SPDArrayPtr);

    float getFloatValue();

    struct SPDStruct* getSPDArray();
    void* getDataPtr();
    int getVarIndex();

    virtual const char* getLabelString() = 0;
    virtual const char* getUnitsString() = 0;
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
