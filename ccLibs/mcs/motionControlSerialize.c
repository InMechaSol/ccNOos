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

void LinkAxisSPDStructArray(struct axisStruct* mystruct, struct SPDStruct* myArray)
{
    for (int i = mcsNone; i < mcsEND; i++)
    {
        switch (i)
        {
        case mcsNone: myArray[i] =  {i,             "",                     "",                 1,NULL_TYPE,    0,                                              nullptr};                               break;
        case mcsPlndT: myArray[i] = {i,             "dT",                   "(sec)",            1,FLOAT_TYPE,   sizeof(mystruct->Planning.dT),                  &mystruct->Planning.dT};                break;
        case mcsPlnTalpha: myArray[i] = {i,         "Talpha",               "(sec)",            1,FLOAT_TYPE,   sizeof(mystruct->Planning.Talpha),              &mystruct->Planning.Talpha};            break;
        case mcsPlnTomega: myArray[i] = {i,         "Tomega",               "(sec)",            1,FLOAT_TYPE,   sizeof(mystruct->Planning.Tomega),              &mystruct->Planning.Tomega};            break;
        case mcsPlnTMotion: myArray[i] = {i,        "Tmotion",              "(sec)",            1,FLOAT_TYPE,   sizeof(mystruct->Planning.Tmotion),             &mystruct->Planning.Tmotion};           break;
        case mcsPlnDeltaPos: myArray[i] = {i,       "Delta Position",       "(rad)",            1,FLOAT_TYPE,   sizeof(mystruct->Planning.delataPos),           &mystruct->Planning.delataPos};         break;
        case mcsDesiredPos: myArray[i] = {i,        "Desired Position",     "(rad)",            0,FLOAT_TYPE,   sizeof(mystruct->Planning.desiredPos),          &mystruct->Planning.desiredPos};        break;
        case mcsDesiredVel: myArray[i] = {i,        "Desired Velocity",     "(rad/sec)",        0,FLOAT_TYPE,   sizeof(mystruct->Planning.motionVelocity),      &mystruct->Planning.motionVelocity};    break;
        case mcsDesiredAcc: myArray[i] = {i,        "Desired Acceleration", "(rad/sec/sec)",    0,FLOAT_TYPE,   sizeof(mystruct->Planning.motionAcceleration),  &mystruct->Planning.motionAcceleration};break;
        case mcsActMotionState: myArray[i] = {i,    "Actual Motion State",  "",                 1,SIGNED_TYPE,  sizeof(mystruct->Planning.actualMotionState),   &mystruct->Planning.actualMotionState}; break;
        case mcsDesiredMotionState: myArray[i] = {i,"Desired Motion State", "",                 1,SIGNED_TYPE,  sizeof(mystruct->Planning.desiredMotionState),  &mystruct->Planning.desiredMotionState};break;
        case mcsUseEstimatedVel: myArray[i] = {i,   "Use Est. Velocity",    "(yes/no)",         0,UNSIGNED_TYPE,sizeof(mystruct->Planning.useEstimatedVelocity),   &mystruct->Planning.useEstimatedVelocity}; break;
        case mcsEstimatedVelocity: myArray[i] = {i, "Estimated Velocity",   "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct->Planning.estVelocity),         &mystruct->Planning.estVelocity};       break;
        case mcsCommandVelocity: myArray[i] = { i,  "Command Velocity",     "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct->Planning.cmdVelocity),         &mystruct->Planning.cmdVelocity};       break;
        case mcsLastPosition: myArray[i] = { i,     "Last Position",        "(rad)",            1,FLOAT_TYPE,   sizeof(mystruct->Planning.LastFbkPosition),     &mystruct->Planning.LastFbkPosition};   break;
        case mcsPositionResolution: myArray[i] = {i,"Position Resolution",  "(rad)",            0,FLOAT_TYPE,   sizeof(mystruct->Planning.PositionResolution),  &mystruct->Planning.PositionResolution};break;

        case mcsActualControlMode: myArray[i] = {i, "Actual Control Mode",  "",                 1,SIGNED_TYPE,  sizeof(mystruct->Planning.actualControlMode),   &mystruct->Planning.actualControlMode}; break;
        case mcsDesiredControlMode: myArray[i]={i,  "Desired Control Mode", "",                 0,SIGNED_TYPE,  sizeof(mystruct->Planning.desiredControlMode),  &mystruct->Planning.desiredControlMode};break;
        
        case mcsCMDgenOutput: myArray[i] = { i,     "Gen. Output",          "",                 1,FLOAT_TYPE,   sizeof(mystruct->Planning.cmdGenerator.cmdOutput),  &mystruct->Planning.cmdGenerator.cmdOutput };       break;
        case mcsCMDgenActualMode: myArray[i] = { i, "Gen. Mode Actual",     "",                 1,UNSIGNED_TYPE,   sizeof(mystruct->Planning.cmdGenerator.actualGenMode),&mystruct->Planning.cmdGenerator.actualGenMode };       break;
        case mcsCMDgenDesiredMode: myArray[i] = { i,"Gen. Mode Desired",    "",                 0,UNSIGNED_TYPE,   sizeof(mystruct->Planning.cmdGenerator.desiredGenMode), &mystruct->Planning.cmdGenerator.desiredGenMode };      break;
        case mcsCMDgenAmplitude: myArray[i] = { i,  "Gen. Amplitude",       "",                 0,FLOAT_TYPE,   sizeof(mystruct->Planning.cmdGenerator.amplitude),&mystruct->Planning.cmdGenerator.amplitude };           break;
        case mcsCMDgenPeriod: myArray[i] = { i,     "Gen. Period",          "(sec)",            0,FLOAT_TYPE,   sizeof(mystruct->Planning.cmdGenerator.period), &mystruct->Planning.cmdGenerator.period };              break;
        case mcsCMDgenDutyCycle: myArray[i] = { i,  "Gen. DutyCycle",       "(0-1)",            0,FLOAT_TYPE,   sizeof(mystruct->Planning.cmdGenerator.dutycyle),&mystruct->Planning.cmdGenerator.dutycyle };            break;
            
        case mcsPosCMD: myArray[i] = {i,            "Position CMD",         "(rad)",            0,FLOAT_TYPE,   sizeof(mystruct->Position.Cmd),                 &mystruct->Position.Cmd};               break;
        case mcsPosFBK: myArray[i] = { i,           "Position FBK",         "(rad)",            1,FLOAT_TYPE,   sizeof(mystruct->Position.Fbk),                 &mystruct->Position.Fbk};               break;
        case mcsPosERR: myArray[i] = { i,           "Position ERR",         "(rad)",            1,FLOAT_TYPE,   sizeof(mystruct->Position.Err),                 &mystruct->Position.Err};               break;
        case mcsPosPLim: myArray[i] = { i,          "Position +Limit",      "(rad)",            0,FLOAT_TYPE,   sizeof(mystruct->Position.LimitPos),            &mystruct->Position.LimitPos};          break;
        case mcsPosNLim: myArray[i] = { i,          "Position -Limit",      "(rad)",            0,FLOAT_TYPE,   sizeof(mystruct->Position.LimitNeg),            &mystruct->Position.LimitNeg};          break;
        case mcsPosCtrldT: myArray[i] = { i,        "PosLoop dT",           "(sec)",            1,FLOAT_TYPE,   sizeof(mystruct->PosController.dT),             &mystruct->PosController.dT};           break;
        case mcsPosCtrlKv: myArray[i] = { i,        "PosLoop Kv",           "(1/sec)",          0,FLOAT_TYPE,   sizeof(mystruct->PosController.Kv),             &mystruct->PosController.Kv};           break;
        case mcsPosCtrlcmdLast: myArray[i] = { i,   "PosLoop CMDLast",      "(rad)",            1,FLOAT_TYPE,   sizeof(mystruct->PosController.cmdLast),        &mystruct->PosController.cmdLast};      break;
        case mcsPosCtrlcmdVel: myArray[i] = { i,    "PosLoop CMDVel",       "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct->PosController.cmdVel),         &mystruct->PosController.cmdVel};       break;
        case mcsVelCMD: myArray[i] = { i,           "Velocity CMD",         "(rad/sec)",        0,FLOAT_TYPE,   sizeof(mystruct->Velocity.Cmd),                 &mystruct->Velocity.Cmd};               break;
        case mcsVelFBK: myArray[i] = { i,           "Velocity FBK",         "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct->Velocity.Fbk),                 &mystruct->Velocity.Fbk};               break;
        case mcsVelERR: myArray[i] = { i,           "Velocity ERR",         "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct->Velocity.Err),                 &mystruct->Velocity.Err};               break;
        case mcsVelPLim: myArray[i] = { i,          "Velocity +Limit",      "(rad/sec)",        0,FLOAT_TYPE,   sizeof(mystruct->Velocity.LimitPos),            &mystruct->Velocity.LimitPos};          break;
        case mcsVelNLim: myArray[i] = { i,          "Velocity -Limit",      "(rad/sec)",        0,FLOAT_TYPE,   sizeof(mystruct->Velocity.LimitNeg),            &mystruct->Velocity.LimitNeg};          break;
        case mcsVelCtrldT: myArray[i] = { i,        "VelLoop dT",           "(sec)",            1,FLOAT_TYPE,   sizeof(mystruct->VelController.dT),             &mystruct->VelController.dT};           break;
        case mcsVelCtrlErr: myArray[i] = { i,       "VelLoop Error",        "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct->VelController.xn),             &mystruct->VelController.xn};           break;
        case mcsVelCtrlErrLast: myArray[i] = { i,   "VelLoop ErrorLast",    "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct->VelController.xn1),            &mystruct->VelController.xn1};          break;
        case mcsVelCtrlOut: myArray[i] = { i,       "VelLoop Torque Cmd",   "(Nm)",             0,FLOAT_TYPE,   sizeof(mystruct->VelController.yn),             &mystruct->VelController.yn};           break;
        case mcsVelCtrlOutLast: myArray[i] = { i,   "VelLoop TorCmd Last",  "(Nm)",             1,FLOAT_TYPE,   sizeof(mystruct->VelController.yn1),            &mystruct->VelController.yn1};          break;
        case mcsVelCtrlOutI: myArray[i] = { i,      "VelLoop TorCmd I",     "(Nm)",             1,FLOAT_TYPE,   sizeof(mystruct->VelController.yin),            &mystruct->VelController.yin};          break;
        case mcsVelCtrlOutP: myArray[i] = { i,      "VelLoop TorCmd P",     "(Nm)",             1,FLOAT_TYPE,   sizeof(mystruct->VelController.ypn),            &mystruct->VelController.ypn};          break;
        case mcsVelCtrlKp: myArray[i] = { i,        "VelLoop Kp",           "(Nm*sec/rad)",     0,FLOAT_TYPE,   sizeof(mystruct->VelController.Kp),             &mystruct->VelController.Kp};           break;
        case mcsVelCtrlTi: myArray[i] = { i,        "VelLoop Ti",           "(sec)",            0,FLOAT_TYPE,   sizeof(mystruct->VelController.Ti),             &mystruct->VelController.Ti};           break;
        case mcsVelCtrlAlpha: myArray[i] = { i,     "VelLoop Alpha",        "(Nm*sec/rad)",     1,FLOAT_TYPE,   sizeof(mystruct->VelController.alpha),          &mystruct->VelController.alpha};        break;
        case mcsVelCtrlReset: myArray[i] = { i,     "VelLoop Reset",        "(yes/no)",         0,UNSIGNED_TYPE,sizeof(mystruct->VelController.reset),          &mystruct->VelController.reset};        break;
        case mcsVelCtrlSaturated: myArray[i] = { i, "VelLoop Saturated",    "(yes/no)",         1,UNSIGNED_TYPE,sizeof(mystruct->VelController.saturated),      &mystruct->VelController.saturated};    break;
        case mcsCurCMD: myArray[i] = { i,           "Current CMD",          "(A)",              0,FLOAT_TYPE,   sizeof(mystruct->Current.Cmd),                  &mystruct->Current.Cmd};                break;
        case mcsCurFBK: myArray[i] = { i,           "Current FBK",          "(A)",              1,FLOAT_TYPE,   sizeof(mystruct->Current.Fbk),                  &mystruct->Current.Fbk};                break;
        case mcsCurERR: myArray[i] = { i,           "Current ERR",          "(A)",              1,FLOAT_TYPE,   sizeof(mystruct->Current.Err),                  &mystruct->Current.Err};                break;
        case mcsCurPLim: myArray[i] = { i,          "Current +Limit",       "(A)",              0,FLOAT_TYPE,   sizeof(mystruct->Current.LimitPos),             &mystruct->Current.LimitPos};           break;
        case mcsCurNLim: myArray[i] = { i,          "Current -Limit",       "(A)",              0,FLOAT_TYPE,   sizeof(mystruct->Current.LimitNeg),             &mystruct->Current.LimitNeg};           break;
        case mcsMotConTorque: myArray[i] = { i,     "Motor Cont. Torque",   "(Nm)",             0,FLOAT_TYPE,   sizeof(mystruct->MotorModel.ContinousTorque),   &mystruct->MotorModel.ContinousTorque}; break;
        case mcsMotVcc: myArray[i] = { i,           "Motor Vcc",            "(V)",              0,FLOAT_TYPE,   sizeof(mystruct->MotorModel.Vcc),               &mystruct->MotorModel.Vcc};             break;
        case mcsMotdT: myArray[i] = { i,            "Motor dT",             "(sec)",            0,FLOAT_TYPE,   sizeof(mystruct->MotorModel.dT),                &mystruct->MotorModel.dT};              break;
        case mcsMotTime: myArray[i] = { i,          "Motor Real Time",      "(sec)",            1,FLOAT_TYPE,   sizeof(mystruct->MotorModel.Time),              &mystruct->MotorModel.Time};            break;
        case mcsMotReset: myArray[i] = { i,         "Motor Reset",          "(yes/no)",         0,UNSIGNED_TYPE,sizeof(mystruct->MotorModel.Reset),             &mystruct->MotorModel.Reset};           break;
        case mcsMotWMinRes: myArray[i] = { i,       "Motor Vel. Res.",      "(rad/sec)",        0,FLOAT_TYPE,   sizeof(mystruct->MotorModel.WminResolution),    &mystruct->MotorModel.WminResolution};  break;
        case mcsMotIdot: myArray[i] = { i,          "Motor Cur. Delta",     "(A/sec)",          1,FLOAT_TYPE,   sizeof(mystruct->MotorModel.Idot),              &mystruct->MotorModel.Idot};            break;
        case mcsMotI: myArray[i] = { i,             "Motor Current",        "(A)",              1,FLOAT_TYPE,   sizeof(mystruct->MotorModel.I),                 &mystruct->MotorModel.I};               break;
        case mcsMotWdot: myArray[i] = { i,          "Motor Rotor Acc.",     "(rad/sec/sec)",    1,FLOAT_TYPE,   sizeof(mystruct->MotorModel.Wdot),              &mystruct->MotorModel.Wdot};            break;
        case mcsMotW: myArray[i] = { i,             "Motor Rotor Vel.",     "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct->MotorModel.W),                 &mystruct->MotorModel.W};               break;
        case mcsMotJ: myArray[i] = { i,             "Motor Rotor J",        "(Kg*m*m)",         0,FLOAT_TYPE,   sizeof(mystruct->MotorModel.J),                 &mystruct->MotorModel.J};               break;
        case mcsMotR: myArray[i] = { i,             "Motor Rotor R",        "(Ohms)",           0,FLOAT_TYPE,   sizeof(mystruct->MotorModel.R),                 &mystruct->MotorModel.R};               break;
        case mcsMotL: myArray[i] = { i,             "Motor Rotor Inductance","(Henrys)",        0,FLOAT_TYPE,   sizeof(mystruct->MotorModel.L),                 &mystruct->MotorModel.L};               break;
        case mcsMotKm: myArray[i] = { i,            "Motor Constant",       "(A/Nm)",           0,FLOAT_TYPE,   sizeof(mystruct->MotorModel.Km),                &mystruct->MotorModel.Km};              break;
        case mcsMotV: myArray[i] = { i,             "Motor Voltage",        "(V)",              0,FLOAT_TYPE,   sizeof(mystruct->MotorModel.V),                 &mystruct->MotorModel.V};               break;
        case mcsMotVemf: myArray[i] = { i,          "Motor Back EMF",       "(V)",              1,FLOAT_TYPE,   sizeof(mystruct->MotorModel.Vemf),              &mystruct->MotorModel.Vemf};            break;
        case mcsMotTorque: myArray[i] = { i,        "Motor Rotor Torque",   "(Nm)",             1,FLOAT_TYPE,   sizeof(mystruct->MotorModel.Torque),            &mystruct->MotorModel.Torque};          break;
        case mcsMotFricTor: myArray[i] = { i,       "Motor Rotor Friction", "(Nm)",             1,FLOAT_TYPE,   sizeof(mystruct->MotorModel.FrictionTorque),    &mystruct->MotorModel.FrictionTorque};  break;
        case mcsMotEff: myArray[i] = { i,           "Motor Efficiency",     "(0 to 1)",         0,FLOAT_TYPE,   sizeof(mystruct->MotorModel.Efficiency),        &mystruct->MotorModel.Efficiency};      break;
        case mcsCurCtrldT: myArray[i] = { i,        "CurLoop dT",           "(sec)",            1,FLOAT_TYPE,   sizeof(mystruct->CurController.dT),             &mystruct->CurController.dT};           break;
        case mcsCurCtrlErr: myArray[i] = { i,       "CurLoop Error",        "(A)",              1,FLOAT_TYPE,   sizeof(mystruct->CurController.xn),             &mystruct->CurController.xn};           break;
        case mcsCurCtrlErrLast: myArray[i] = { i,   "CurLoop ErrorLast",    "(A)",              1,FLOAT_TYPE,   sizeof(mystruct->CurController.xn1),            &mystruct->CurController.xn1};          break;
        case mcsCurCtrlOut: myArray[i] = { i,       "CurLoop Voltage Cmd",  "(V)",              1,FLOAT_TYPE,   sizeof(mystruct->CurController.yn),             &mystruct->CurController.yn};           break;
        case mcsCurCtrlOutLast: myArray[i] = { i,   "CurLoop VoltsCmd Last","(V)",              1,FLOAT_TYPE,   sizeof(mystruct->CurController.yn1),            &mystruct->CurController.yn1};          break;
        case mcsCurCtrlOutI: myArray[i] = { i,      "CurLoop VoltsCmd I",   "(V)",              1,FLOAT_TYPE,   sizeof(mystruct->CurController.yin),            &mystruct->CurController.yin};          break;
        case mcsCurCtrlOutP: myArray[i] = { i,      "CurLoop VoltsCmd P",   "(V)",              1,FLOAT_TYPE,   sizeof(mystruct->CurController.ypn),            &mystruct->CurController.ypn};          break;
        case mcsCurCtrlKp: myArray[i] = { i,        "CurLoop Kp",           "(V/A)",            0,FLOAT_TYPE,   sizeof(mystruct->CurController.Kp),             &mystruct->CurController.Kp};           break;
        case mcsCurCtrlTi: myArray[i] = { i,        "CurLoop Ti",           "(sec)",            0,FLOAT_TYPE,   sizeof(mystruct->CurController.Ti),             &mystruct->CurController.Ti};           break;
        case mcsCurCtrlAlpha: myArray[i] = { i,     "CurLoop Alpha",        "(V/A)",            1,FLOAT_TYPE,   sizeof(mystruct->CurController.alpha),          &mystruct->CurController.alpha};        break;
        case mcsCurCtrlReset: myArray[i] = { i,     "CurLoop Reset",        "(yes/no)",         0,UNSIGNED_TYPE,sizeof(mystruct->CurController.reset),          &mystruct->CurController.reset};        break;
        case mcsCurCtrlSaturated: myArray[i] = { i, "CurLoop Saturated",    "(yes/no)",         0,UNSIGNED_TYPE,sizeof(mystruct->CurController.saturated),      &mystruct->CurController.saturated};    break;
        case mcsPWMCMD: myArray[i] = { i,           "PWM Command",          "(-1 to 1)",        0,FLOAT_TYPE,   sizeof(mystruct->PWMCmd),                       &mystruct->PWMCmd };                    break;
        case mcsPWMLimit: myArray[i] = { i,         "PWM Limit",            "(0 to 1)",         0,FLOAT_TYPE,   sizeof(mystruct->PWMLimit),                     &mystruct->PWMLimit };                  break;
        case mcsPWMSaturated: myArray[i] = { i,     "PWM Saturated",        "(1 / 0)",          1,FLOAT_TYPE,   sizeof(mystruct->PWMSaturated),                 &mystruct->PWMSaturated };              break;

        case mcsCurrentSaturated: myArray[i] = { i, "Current Saturated",    "(1 / 0)",          1,UNSIGNED_TYPE,sizeof(mystruct->CurrentSaturated),             &mystruct->CurrentSaturated };          break;
        case mcsCtrlEnabled: myArray[i] = { i,      "Enable Control",       "(1 / 0)",          0,UNSIGNED_TYPE,sizeof(mystruct->ctrlEnabled),                  &mystruct->ctrlEnabled };               break;
        case mcsCurrentCtrlEnabled: myArray[i] = { i,"Enable Current Control","(1 / 0)",        0,UNSIGNED_TYPE,sizeof(mystruct->currentCtrlEnabled),           &mystruct->currentCtrlEnabled };               break;
        case mcsTorqueCmd: myArray[i] = { i,        "Torque CMD",           "(Nm)",             0,FLOAT_TYPE,   sizeof(mystruct->torqueCmd),                    &mystruct->torqueCmd };                 break;
        case mcsVoltageCmd: myArray[i] = { i,       "Voltage CMD",          "(V)",              0,FLOAT_TYPE,   sizeof(mystruct->voltageCmd),                   &mystruct->voltageCmd };                break;
        case mcsVoltageLimit: myArray[i] = { i,     "Voltage Limit",        "(V)",              0,FLOAT_TYPE,   sizeof(mystruct->voltageLimit),                 &mystruct->voltageLimit };              break;
        
        }
        
    }
}
