/** \file motionControl.c
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

#include "motionControlSerialize.h"
#include "motionControl.h"


#define L(Index, Label, Units, readOnly, Type, Var) myArray[Index] = {Index, Label, Units, readOnly, Type, sizeof(Var), &Var}


void LinkAxisSPDStructArray(struct axisStruct* mystruct, struct SPDStruct* myArray)
{

L(mcsAxCtrlEnabled,     "Enable Control",       "(1/0)",                        0,UNSIGNED_TYPE,mystruct->ctrlEnabled  );
L(mcsAxIgnoreEE,        "Direct Load Control",  "(1/0)",                        0,UNSIGNED_TYPE,mystruct->ignoreEE  );
L(mcsAxReset,           "Reset",                "(1/0)",                        0,UNSIGNED_TYPE,mystruct->Reset  );
L(mcsAxTime,            "Real Time",            "(sec)",                        1,FLOAT_TYPE,   mystruct->Time );

L(mcsPlndT,             "dT",                   "(sec)",                        1,FLOAT_TYPE,   mystruct->Planning.dT );
L(mcsDesiredPos,        "Desired Position",     "(rad)",                        0,FLOAT_TYPE,   mystruct->Planning.desiredPos );
L(mcsDesiredVel,        "Desired Velocity",     "(rad/sec)",                    0,FLOAT_TYPE,   mystruct->Planning.desiredVelocity );
L(mcsDesiredAcc,        "Desired Acceleration", "(rad/sec/sec)",                0,FLOAT_TYPE,   mystruct->Planning.desiredAcceleration );

L(mcsPlnTalpha,         "Talpha",               "(sec)",                        1,FLOAT_TYPE,   mystruct->Planning.Talpha );
L(mcsPlnTomega,         "Tomega",               "(sec)",                        1,FLOAT_TYPE,   mystruct->Planning.Tomega );
L(mcsPlnTMotion,        "Tmotion",              "(sec)",                        1,FLOAT_TYPE,   mystruct->Planning.Tmotion );
L(mcsPlnDeltaPos,       "Delta Position",       "(rad)",                        1,FLOAT_TYPE,   mystruct->Planning.delataPos );


L(mcsActMotionState,    "Actual Motion State",  "(-1,0,1 neg-stop-pos)",        1,SIGNED_TYPE,  mystruct->Planning.actualMotionState );
L(mcsDesiredMotionState,"Desired Motion State", "(-1,0,1 neg-stop-pos)",        1,SIGNED_TYPE,  mystruct->Planning.desiredMotionState );
L(mcsUseEstimatedVel,   "Use Est. Velocity",    "(yes/no)",                     0,UNSIGNED_TYPE,mystruct->Planning.useEstimatedVelocity );
L(mcsEstimatedVelocity, "Estimated Velocity",   "(rad/sec)",                    1,FLOAT_TYPE,   mystruct->Planning.estVelocity );
L(mcsCommandVelocity,   "Command Velocity",     "(rad/sec)",                    1,FLOAT_TYPE,   mystruct->Planning.cmdVelocity );
L(mcsLastPosition,      "Last Position",        "(rad)",                        1,FLOAT_TYPE,   mystruct->Planning.LastFbkPosition );
L(mcsPositionResolution,"Position Resolution",  "(rad)",                        0,FLOAT_TYPE,   mystruct->Planning.PositionResolution );
L(mcsActualControlMode,  "Actual Control Mode",  "(0-3 pos-vel-cur-pwm)",       1,SIGNED_TYPE,  mystruct->Planning.actualControlMode );
L(mcsDesiredControlMode, "Desired Control Mode", "(0-3 pos-vel-cur-pwm)",       0,SIGNED_TYPE,  mystruct->Planning.desiredControlMode );

L(mcsCMDgenOutput,       "Gen. Output",          "",                            1,FLOAT_TYPE,   mystruct->Planning.cmdGenerator.cmdOutput  );
L(mcsCMDgenActualMode,   "Gen. Mode Actual",     "",                            1,UNSIGNED_TYPE,mystruct->Planning.cmdGenerator.actualGenMode  );
L(mcsCMDgenDesiredMode,  "Gen. Mode Desired",    "(0-3 none-pulse-train-sine)", 0,UNSIGNED_TYPE,mystruct->Planning.cmdGenerator.desiredGenMode );
L(mcsCMDgenOffset,       "Gen. Offset",          "(0-3 none-pulse-train-sine)", 0,FLOAT_TYPE,   mystruct->Planning.cmdGenerator.offset  );
L(mcsCMDgenAmplitude,    "Gen. Amplitude",       "",                            0,FLOAT_TYPE,   mystruct->Planning.cmdGenerator.amplitude  );
L(mcsCMDgenPeriod,       "Gen. Period",          "(sec)",                       0,FLOAT_TYPE,   mystruct->Planning.cmdGenerator.period );
L(mcsCMDgenDutyCycle,    "Gen. DutyCycle",       "(0-1)",                       0,FLOAT_TYPE,   mystruct->Planning.cmdGenerator.dutycyle  );
            
L(mcsPosCMD,             "Position CMD",         "(rad)",                       0,FLOAT_TYPE,   mystruct->Position.Cmd );
L(mcsPosFBK,             "Position FBK",         "(rad)",                       1,FLOAT_TYPE,   mystruct->Position.Fbk );
L(mcsPosERR,             "Position ERR",         "(rad)",                       1,FLOAT_TYPE,   mystruct->Position.Err );
L(mcsPosPLim,            "Position +Limit",      "(rad)",                       0,FLOAT_TYPE,   mystruct->Position.LimitPos );
L(mcsPosNLim,            "Position -Limit",      "(rad)",                       0,FLOAT_TYPE,   mystruct->Position.LimitNeg );
L(mcsPosCtrldT,          "PosLoop dT",           "(sec)",                       1,FLOAT_TYPE,   mystruct->PosController.dT );
L(mcsPosCtrlKv,          "PosLoop Kv",           "(1/sec)",                     0,FLOAT_TYPE,   mystruct->PosController.Kv );
L(mcsPosCtrlcmdLast,     "PosLoop CMDLast",      "(rad)",                       1,FLOAT_TYPE,   mystruct->PosController.cmdLast );
L(mcsPosCtrlcmdVel,      "PosLoop CMDVel",       "(rad/sec)",                   1,FLOAT_TYPE,   mystruct->PosController.cmdVel );
L(mcsVelCMD,             "Velocity CMD",         "(rad/sec)",                   0,FLOAT_TYPE,   mystruct->Velocity.Cmd );
L(mcsVelFBK,             "Velocity FBK",         "(rad/sec)",                   1,FLOAT_TYPE,   mystruct->Velocity.Fbk );
L(mcsVelERR,             "Velocity ERR",         "(rad/sec)",                   1,FLOAT_TYPE,   mystruct->Velocity.Err );
L(mcsVelPLim,            "Velocity +Limit",      "(rad/sec)",                   0,FLOAT_TYPE,   mystruct->Velocity.LimitPos );
L(mcsVelNLim,            "Velocity -Limit",      "(rad/sec)",                   0,FLOAT_TYPE,   mystruct->Velocity.LimitNeg );
L(mcsVelCtrldT,          "VelLoop dT",           "(sec)",                       1,FLOAT_TYPE,   mystruct->VelController.dT );
L(mcsVelCtrlErr,         "VelLoop Error",        "(rad/sec)",                   1,FLOAT_TYPE,   mystruct->VelController.xn );
L(mcsVelCtrlErrLast,     "VelLoop ErrorLast",    "(rad/sec)",                   1,FLOAT_TYPE,   mystruct->VelController.xn1 );
L(mcsVelCtrlOut,         "VelLoop Torque Cmd",   "(Nm)",                        0,FLOAT_TYPE,   mystruct->VelController.yn );
L(mcsVelCtrlOutLast,     "VelLoop TorCmd Last",  "(Nm)",                        1,FLOAT_TYPE,   mystruct->VelController.yn1 );
L(mcsVelCtrlOutI,        "VelLoop TorCmd I",     "(Nm)",                        1,FLOAT_TYPE,   mystruct->VelController.yin );
L(mcsVelCtrlOutP,        "VelLoop TorCmd P",     "(Nm)",                        1,FLOAT_TYPE,   mystruct->VelController.ypn );
L(mcsVelCtrlKp,          "VelLoop Kp",           "(Nm*sec/rad)",                0,FLOAT_TYPE,   mystruct->VelController.Kp );
L(mcsVelCtrlTi,          "VelLoop Ti",           "(sec)",                       0,FLOAT_TYPE,   mystruct->VelController.Ti );
L(mcsVelCtrlAlpha,       "VelLoop Alpha",        "(Nm*sec/rad)",                1,FLOAT_TYPE,   mystruct->VelController.alpha );
L(mcsVelCtrlReset,       "VelLoop Reset",        "(yes/no)",                    0,UNSIGNED_TYPE,mystruct->VelController.reset );
L(mcsVelCtrlSaturated,   "VelLoop Saturated",    "(yes/no)",                    1,UNSIGNED_TYPE,mystruct->VelController.saturated );


L(mcsLoadAminRes,           "Load AminRes" ,        "(rad/sec/sec)",    1,FLOAT_TYPE, mystruct->AxisLoadInertia.AminResolution );
L(mcsLoadWminRes,           "Load WminRes" ,        "(rad/sec)",        1,FLOAT_TYPE, mystruct->AxisLoadInertia.WminResolution );
L(mcsLoaddT,                "Load dT" ,             "(sec)",            1,FLOAT_TYPE, mystruct->AxisLoadInertia.dT );
L(mcsLoadOmegaDot,          "Load OmegaDot" ,       "(rad/sec/sec)",    1,FLOAT_TYPE, mystruct->AxisLoadInertia.OmegaDot );
L(mcsLoadOmega,             "Load Omega" ,          "(rad/sec)",        1,FLOAT_TYPE, mystruct->AxisLoadInertia.Omega );
L(mcsLoadThetaDot,          "Load ThetaDot" ,       "(rad/sec)",        1,FLOAT_TYPE, mystruct->AxisLoadInertia.ThetaDot );
L(mcsLoadTheta,             "Load Theta" ,          "(rad)",            1,FLOAT_TYPE, mystruct->AxisLoadInertia.Theta );
L(mcsLoadTorqueApplied,     "Load Torque Applied",  "(Nm)",             0,FLOAT_TYPE, mystruct->AxisLoadInertia.TorqueApplied );
L(mcsLoadTorqueFriction,    "Load Torque Friction", "(Nm)",             1,FLOAT_TYPE, mystruct->AxisLoadInertia.TorqueFriction );
L(mcsLoadEfficiency,        "Load Efficiency" ,     "(0-1)",            0,FLOAT_TYPE, mystruct->AxisLoadInertia.Efficiency );
L(mcsLoadKf,                "Load Fric. Kf" ,       "(Nm)",             1,FLOAT_TYPE, mystruct->AxisLoadInertia.Kf );
L(mcsLoadRf,                "Load Fric. Rf" ,       "(0-1)",            0,FLOAT_TYPE, mystruct->AxisLoadInertia.Rf );
L(mcsLoadTorqueRating,      "Load Rated Torque" ,   "(Nm)",             0,FLOAT_TYPE, mystruct->AxisLoadInertia.TorqueRating );
L(mcsLoadInertia,           "Load Inertia" ,        "(kg/m/m)",         0,FLOAT_TYPE, mystruct->AxisLoadInertia.Inertia );
L(mcsLoadReset,             "Load Reset" ,          "(1/0)",            0,UNSIGNED_TYPE, mystruct->AxisLoadInertia.Reset );
L(mcsLoadEstFriction,       "Load Est. Fric." ,     "(1/0)",            0,UNSIGNED_TYPE, mystruct->AxisLoadInertia.EstFriction );

L(mcsCurCMD,             "Current CMD",          "(A)",              0,FLOAT_TYPE,   mystruct->Current.Cmd );
L(mcsCurFBK,             "Current FBK",          "(A)",              1,FLOAT_TYPE,   mystruct->Current.Fbk );
L(mcsCurERR,             "Current ERR",          "(A)",              1,FLOAT_TYPE,   mystruct->Current.Err );
L(mcsCurPLim,            "Current +Limit",       "(A)",              0,FLOAT_TYPE,   mystruct->Current.LimitPos );
L(mcsCurNLim,            "Current -Limit",       "(A)",              0,FLOAT_TYPE,   mystruct->Current.LimitNeg );
L(mcsMotConTorque,       "Motor Cont. Torque",   "(Nm)",             0,FLOAT_TYPE,   mystruct->MotorModel.ContinousTorque );
L(mcsMotVcc,             "Motor Vcc",            "(V)",              0,FLOAT_TYPE,   mystruct->MotorModel.Vcc );
L(mcsMotdT,             "Motor dT",             "(sec)",            0,FLOAT_TYPE,   mystruct->MotorModel.dT );

L(mcsMotReset,           "Motor Reset",          "(yes/no)",         0,UNSIGNED_TYPE,mystruct->MotorModel.Reset );
L(mcsMotWMinRes,         "Motor Vel. Res.",      "(rad/sec)",        0,FLOAT_TYPE,   mystruct->MotorModel.WminResolution );
L(mcsMotIdot,            "Motor Cur. Delta",     "(A/sec)",          1,FLOAT_TYPE,   mystruct->MotorModel.Idot );
L(mcsMotI,               "Motor Current",        "(A)",              1,FLOAT_TYPE,   mystruct->MotorModel.I );
L(mcsMotWdot,            "Motor Rotor Acc.",     "(rad/sec/sec)",    1,FLOAT_TYPE,   mystruct->MotorModel.Wdot );
L(mcsMotW,               "Motor Rotor Vel.",     "(rad/sec)",        1,FLOAT_TYPE,   mystruct->MotorModel.W );
L(mcsMotJ,               "Motor Rotor J",        "(Kg*m*m)",         0,FLOAT_TYPE,   mystruct->MotorModel.J );
L(mcsMotR,               "Motor Rotor R",        "(Ohms)",           0,FLOAT_TYPE,   mystruct->MotorModel.R );
L(mcsMotL,               "Motor Rotor Inductance","(Henrys)",        0,FLOAT_TYPE,   mystruct->MotorModel.L );
L(mcsMotKm,              "Motor Constant",       "(A/Nm)",           0,FLOAT_TYPE,   mystruct->MotorModel.Km );
L(mcsMotV,               "Motor Voltage",        "(V)",              0,FLOAT_TYPE,   mystruct->MotorModel.V );
L(mcsMotVemf,            "Motor Back EMF",       "(V)",              1,FLOAT_TYPE,   mystruct->MotorModel.Vemf );
L(mcsMotTorque,          "Motor Rotor Torque",   "(Nm)",             1,FLOAT_TYPE,   mystruct->MotorModel.Torque );
L(mcsMotFricTor,         "Motor Rotor Friction", "(Nm)",             1,FLOAT_TYPE,   mystruct->MotorModel.FrictionTorque );
L(mcsMotEff,             "Motor Efficiency",     "(0 to 1)",         0,FLOAT_TYPE,   mystruct->MotorModel.Efficiency );

L(mcsCurCtrldT,          "CurLoop dT",           "(sec)",            1,FLOAT_TYPE,   mystruct->CurController.dT );
L(mcsCurCtrlErr,         "CurLoop Error",        "(A)",              1,FLOAT_TYPE,   mystruct->CurController.xn );
L(mcsCurCtrlErrLast,     "CurLoop ErrorLast",    "(A)",              1,FLOAT_TYPE,   mystruct->CurController.xn1 );
L(mcsCurCtrlOut,         "CurLoop Voltage Cmd",  "(V)",              1,FLOAT_TYPE,   mystruct->CurController.yn );
L(mcsCurCtrlOutLast,     "CurLoop VoltsCmd Last","(V)",              1,FLOAT_TYPE,   mystruct->CurController.yn1 );
L(mcsCurCtrlOutI,        "CurLoop VoltsCmd I",   "(V)",              1,FLOAT_TYPE,   mystruct->CurController.yin );
L(mcsCurCtrlOutP,        "CurLoop VoltsCmd P",   "(V)",              1,FLOAT_TYPE,   mystruct->CurController.ypn );
L(mcsCurCtrlKp,          "CurLoop Kp",           "(V/A)",            0,FLOAT_TYPE,   mystruct->CurController.Kp );
L(mcsCurCtrlTi,          "CurLoop Ti",           "(sec)",            0,FLOAT_TYPE,   mystruct->CurController.Ti );
L(mcsCurCtrlAlpha,       "CurLoop Alpha",        "(V/A)",            1,FLOAT_TYPE,   mystruct->CurController.alpha );
L(mcsCurCtrlReset,       "CurLoop Reset",        "(yes/no)",         0,UNSIGNED_TYPE,mystruct->CurController.reset );
L(mcsCurCtrlSaturated,   "CurLoop Saturated",    "(yes/no)",         0,UNSIGNED_TYPE,mystruct->CurController.saturated );

L(mcsPWMCMD,             "PWM Command",          "(-1 to 1)",        0,FLOAT_TYPE,   mystruct->PWMCmd  );
L(mcsPWMLimit,           "PWM Limit",            "(0 to 1)",         0,FLOAT_TYPE,   mystruct->PWMLimit  );
L(mcsPWMSaturated,       "PWM Saturated",        "(1 / 0)",          1,FLOAT_TYPE,   mystruct->PWMSaturated );
L(mcsCurrentSaturated,   "Current Saturated",    "(1 / 0)",          1,UNSIGNED_TYPE,mystruct->CurrentSaturated  );
L(mcsCurrentCtrlEnabled, "Enable Current Control","(1 / 0)",        0,UNSIGNED_TYPE,mystruct->currentCtrlEnabled  );
L(mcsTorqueCmd,          "Torque CMD",           "(Nm)",             0,FLOAT_TYPE,   mystruct->torqueCmd  );
L(mcsVoltageCmd,         "Voltage CMD",          "(V)",              0,FLOAT_TYPE,   mystruct->voltageCmd );
L(mcsVoltageLimit,       "Voltage Limit",        "(V)",              0,FLOAT_TYPE,   mystruct->voltageLimit );

}
