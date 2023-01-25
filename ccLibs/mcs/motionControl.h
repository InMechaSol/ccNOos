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

#include "abstractMATH.h"
#include "packets_api.h"


///
/// \brief The command generator enum
///
enum commandGenEnum
{
    generateNothing = 0,
    generatePulse,
    generatePulseTrain,
    generateSinWave
};

///
/// \brief The controlTypeEnum enum
///
enum controlTypeEnum
{
    controlPosition=0,
    controlVelocity,
    controlCurrent,
    controlPWM
};
///
/// \brief The motionStateEnum enum
///
enum motionStateEnum
{
    motionStopped=0,
    motionPositive,
    motionNegative
};

///
/// \brief The generatorStruct 
///
struct generatorStruct
{
    float cmdOutput;
    UI_16 actualGenMode, desiredGenMode;
    float amplitude, period, dutycyle;
};
struct generatorStruct creategeneratorStruct();

///
/// \brief The planningStruct class
///
struct planningStruct
{
    float dT, delataPos, Talpha, Tomega, Tmotion;
    float desiredPos, motionVelocity, motionAcceleration;
    float estVelocity, cmdVelocity, LastFbkPosition, PositionResolution;
    UI_16 actualMotionState;
    UI_16 desiredMotionState;
    UI_16 actualControlMode;
    UI_16 desiredControlMode;
    UI_8 useEstimatedVelocity;
    struct generatorStruct cmdGenerator;
};
struct planningStruct createplanningStruct();


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
void commandGenerator(struct axisStruct* axisStructPtr);
void planningLoop(struct axisStruct* axisStructPtr);
void positionLoop(struct axisStruct* axisStructPtr);
void velocityLoop(struct axisStruct* axisStructPtr);
void currentLoop(struct axisStruct* axisStructPtr);

#ifdef __cplusplus

class SmartMotorDevice
{
private:

struct axisStruct* mystruct;
struct SPDStruct* AxisSPDStructArray;

protected:
    virtual void readSMData(){return;}
    virtual void writeSMData(){return;}

public:
    SmartMotorDevice(struct axisStruct* mystructIn, struct SPDStruct* AxisSPDStructArrayIn);
    void prepare();
    void execute();
    struct SPDStruct* getSPDArray();
};

#endif
#endif
