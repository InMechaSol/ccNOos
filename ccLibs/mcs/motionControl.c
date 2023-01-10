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

#include "motionControl.h"

///
/// \brief createplanningStruct
/// \return
///
struct planningStruct createplanningStruct()
{
    struct planningStruct outStruct;
    outStruct.dT = 0.001;
    outStruct.delataPos = 0;
    outStruct.Talpha = 0;
    outStruct.Tomega = 0;
    outStruct.Tmotion = 0;
    outStruct.desiredPos = 0;
    outStruct.motionVelocity = 0;
    outStruct.motionAcceleration = 0;
    outStruct.estVelocity = 0;
    outStruct.cmdVelocity = 0;
    outStruct.LastFbkPosition = 0;
    outStruct.PositionResolution = 0.000001;
    outStruct.actualControlMode = controlPWM;
    outStruct.desiredControlMode = controlPWM;
    outStruct.actualMotionState = motionStopped;
    outStruct.desiredMotionState = motionStopped;
    outStruct.useEstimatedVelocity = ui8FALSE;
    return outStruct;
}
///
/// \brief createposConrolStruct
/// \return
///
struct posControlStruct createposConrolStruct()
{
    struct posControlStruct outStruct;
    outStruct.Kv = 0;
    outStruct.dT = 0.001;
    outStruct.cmdLast = 0;
    outStruct.cmdVel = 0;
    return outStruct;
}
///
/// \brief createcontroledFloat
/// \return
///
struct controledFloat createcontroledFloat()
{
    struct controledFloat outStruct;
    outStruct.Cmd = 0.0;
    outStruct.Err = 0.0;
    outStruct.Fbk = 0.0;
    outStruct.LimitNeg = -1;
    outStruct.LimitPos = 1;
    return outStruct;
}

///
/// \brief createpiControllerStruct
/// \return
///
struct piControllerStruct createpiControllerStruct()
{
    struct piControllerStruct outStruct;
    outStruct.dT = 0.001;
    outStruct.Kp = 0;
    outStruct.Ti = 0;
    outStruct.reset = ui8TRUE;
    outStruct.saturated = ui8FALSE;
    outStruct.xn = 0;
    outStruct.yn = 0;
    outStruct.ypn = 0;
    outStruct.yin = 0;
    preparepiControllerStruct(&outStruct);
    return outStruct;
}
void preparepiControllerStruct(struct piControllerStruct* piControllerPtr)
{
    if(piControllerPtr->Ti>0)
        piControllerPtr->alpha = piControllerPtr->Kp*piControllerPtr->dT/(2*piControllerPtr->Ti);
    if(piControllerPtr->reset)
    {
        piControllerPtr->xn1 = 0;
        piControllerPtr->yn1 = 0;
        piControllerPtr->reset = ui8FALSE;
    }
}
void executepiControllerStruct(struct piControllerStruct* piControllerPtr)
{
    piControllerPtr->ypn = piControllerPtr->Kp*piControllerPtr->xn;

    if(piControllerPtr->Ti>0)
    {
        if(!piControllerPtr->saturated)
            piControllerPtr->yin = piControllerPtr->alpha*piControllerPtr->xn + piControllerPtr->alpha*piControllerPtr->xn1 + piControllerPtr->yn1;

    }
    else
        piControllerPtr->yin = 0;
    piControllerPtr->yn = piControllerPtr->yin + piControllerPtr->ypn;

    piControllerPtr->yn1 = piControllerPtr->yin;
    piControllerPtr->xn1 = piControllerPtr->xn;
}

///
/// \brief createdcMotorStruct
/// \return
///
struct dcMotorStruct createdcMotorStruct()
{
    struct dcMotorStruct outStruct;

    // Motor State Differentials
    outStruct.Idot = 0;
    outStruct.Wdot = 0;

    // Motor State Variables
    outStruct.I = 0;
    outStruct.W = 0;

    // Motor Observed Variables (functions of state and parameters)
    outStruct.Vemf = 0;
    outStruct.V = 0;
    outStruct.Torque = 0;
    outStruct.FrictionTorque = 0;

    // Motor Rating Parameters
    outStruct.Efficiency = 0.9; //
    outStruct.ContinousTorque = 0.065; // (Nm) IQ2306

    // Motor Physics Parameters
    outStruct.dT = 0.0000000001; // 0.001 sec
    outStruct.J = 0.000000023; // 0.000000023 (Kg*m*m) estimate from mass and solid cyclinder of r=1.5mm
    outStruct.R = 0.045; // 0.045 (ohms) IQ2306
    outStruct.L = 0.003; // 0.003 (henry) NEMA23 similar spec
    outStruct.Km = 1/0.0043; // 1/0.0043 (A/Nm) IQ2306
    outStruct.WminResolution = 0.0001; // 0.0001 (rad/sec)
    outStruct.Vcc = 24; // 24 (V) line voltage to motor pwm driver

    outStruct.Reset = ui8TRUE;
    outStruct.Time = -outStruct.dT;

    // Calculate Coefficients
    preparedcMotorStruct(&outStruct);

    return outStruct;
}
void preparedcMotorStruct(struct dcMotorStruct* dcMotorStructPtrIn)
{
    // calculate ss coefficiencts from motor parameters
    dcMotorStructPtrIn->a11 = -dcMotorStructPtrIn->R/dcMotorStructPtrIn->L;
    dcMotorStructPtrIn->a12 = -dcMotorStructPtrIn->Km/dcMotorStructPtrIn->L;
    dcMotorStructPtrIn->a21 = 1/(dcMotorStructPtrIn->Km*dcMotorStructPtrIn->J);
    dcMotorStructPtrIn->a22 = 0;
}
void executedcMotorStruct(struct dcMotorStruct* dcMotorStructPtrIn)
{
    if(dcMotorStructPtrIn->Reset)
    {
        dcMotorStructPtrIn->Time = -dcMotorStructPtrIn->dT;
        dcMotorStructPtrIn->Idot = 0;
        dcMotorStructPtrIn->Wdot = 0;
        dcMotorStructPtrIn->I = 0;
        dcMotorStructPtrIn->W = 0;
        dcMotorStructPtrIn->Reset = ui8FALSE;
    }

    // calculate friction estimate from states and efficiency parameter
    if(dcMotorStructPtrIn->W <= dcMotorStructPtrIn->WminResolution && dcMotorStructPtrIn->W >= -dcMotorStructPtrIn->WminResolution)
    {
        // Static Friction Always oppose Motion
        if(dcMotorStructPtrIn->I>0)
            dcMotorStructPtrIn->FrictionTorque = -(dcMotorStructPtrIn->a21*dcMotorStructPtrIn->I + dcMotorStructPtrIn->a22*dcMotorStructPtrIn->W)*dcMotorStructPtrIn->J;
        else if(dcMotorStructPtrIn->I<0)
            dcMotorStructPtrIn->FrictionTorque = (dcMotorStructPtrIn->a21*dcMotorStructPtrIn->I + dcMotorStructPtrIn->a22*dcMotorStructPtrIn->W)*dcMotorStructPtrIn->J;
        else
            dcMotorStructPtrIn->FrictionTorque = 0.0;

        // Static Friction Cut-off Torque
        if(dcMotorStructPtrIn->FrictionTorque < -dcMotorStructPtrIn->ContinousTorque*(1-dcMotorStructPtrIn->Efficiency))
            dcMotorStructPtrIn->FrictionTorque = -dcMotorStructPtrIn->ContinousTorque*(1-dcMotorStructPtrIn->Efficiency);
        else if(dcMotorStructPtrIn->FrictionTorque > dcMotorStructPtrIn->ContinousTorque*(1-dcMotorStructPtrIn->Efficiency))
            dcMotorStructPtrIn->FrictionTorque = dcMotorStructPtrIn->ContinousTorque*(1-dcMotorStructPtrIn->Efficiency);
    }
    else
    {
        // Kinetic Friction
        if(dcMotorStructPtrIn->W > dcMotorStructPtrIn->WminResolution)
        {
            dcMotorStructPtrIn->FrictionTorque = -.25*dcMotorStructPtrIn->ContinousTorque*(1-dcMotorStructPtrIn->Efficiency);
        }
        else if(dcMotorStructPtrIn->W < -dcMotorStructPtrIn->WminResolution)
        {
            dcMotorStructPtrIn->FrictionTorque = .25*dcMotorStructPtrIn->ContinousTorque*(1-dcMotorStructPtrIn->Efficiency);
        }
    }
    dcMotorStructPtrIn->FrictionTorque = 0.0;

    // calculate ss differentials from states, coefficients, and inputs
    dcMotorStructPtrIn->Idot = dcMotorStructPtrIn->a11*dcMotorStructPtrIn->I + dcMotorStructPtrIn->a12*dcMotorStructPtrIn->W + dcMotorStructPtrIn->V*1/dcMotorStructPtrIn->L;
    dcMotorStructPtrIn->Wdot = dcMotorStructPtrIn->a21*dcMotorStructPtrIn->I + dcMotorStructPtrIn->a22*dcMotorStructPtrIn->W + dcMotorStructPtrIn->FrictionTorque*1/dcMotorStructPtrIn->J;

    // calculate ss next states from ss differentials and time delta
    dcMotorStructPtrIn->I += dcMotorStructPtrIn->Idot*dcMotorStructPtrIn->dT;
    dcMotorStructPtrIn->W += dcMotorStructPtrIn->Wdot*dcMotorStructPtrIn->dT;
    //dcMotorStructPtrIn->Time += dcMotorStructPtrIn->dT;

    // calculate observed variables from states and parameters
    dcMotorStructPtrIn->Torque = dcMotorStructPtrIn->I/dcMotorStructPtrIn->Km;
    dcMotorStructPtrIn->Vemf = dcMotorStructPtrIn->Km*dcMotorStructPtrIn->W;



}

///
/// \brief createaxisStruct
/// \return
///
struct axisStruct createaxisStruct()
{
    struct axisStruct outStruct;
    outStruct.Planning = createplanningStruct();
    outStruct.Position = createcontroledFloat();
    outStruct.PosController = createposConrolStruct();
    outStruct.Velocity = createcontroledFloat();
    outStruct.VelController = createpiControllerStruct();
    outStruct.Current = createcontroledFloat();    
    outStruct.MotorModel =  createdcMotorStruct();
    outStruct.CurController = createpiControllerStruct();
    outStruct.torqueCmd = 0;
    outStruct.voltageCmd = 0;
    outStruct.voltageLimit = 0;
    outStruct.PWMCmd = 0;
    outStruct.PWMLimit = 0;
    outStruct.PWMSaturated = 0;
    outStruct.currentCtrlEnabled = 0;
    outStruct.CurrentSaturated = 0;
    outStruct.ctrlEnabled = 0;
    return outStruct;
};

///
/// \brief Cascade Control Loops
///
void planningLoop(struct axisStruct* axisStructPtr)
{
    // always monitor feedback to determine actual motion state
    if((axisStructPtr->Position.Fbk-axisStructPtr->Planning.LastFbkPosition) < axisStructPtr->Planning.PositionResolution)
    {
        axisStructPtr->Planning.actualMotionState = motionNegative;
        axisStructPtr->Planning.estVelocity = (axisStructPtr->Position.Fbk-axisStructPtr->Planning.LastFbkPosition)/axisStructPtr->Planning.dT;
    }
    else if((axisStructPtr->Position.Fbk-axisStructPtr->Planning.LastFbkPosition) > axisStructPtr->Planning.PositionResolution)
    {
        axisStructPtr->Planning.actualMotionState = motionPositive;
        axisStructPtr->Planning.estVelocity = (axisStructPtr->Position.Fbk-axisStructPtr->Planning.LastFbkPosition)/axisStructPtr->Planning.dT;
    }
    else
    {
        axisStructPtr->Planning.actualMotionState = motionStopped;
        axisStructPtr->Planning.estVelocity = 0;
    }
    axisStructPtr->Planning.LastFbkPosition = axisStructPtr->Position.Fbk;
    if(axisStructPtr->ctrlEnabled)
    {
        switch (axisStructPtr->Planning.actualControlMode) {
        case controlPosition:
        {
            // from desired and actual state of motion stopped, determine if new motion should be commanded
            // and calculate critical time boundaries
            if(axisStructPtr->Planning.desiredMotionState == motionStopped && axisStructPtr->Planning.actualMotionState == motionStopped)
            {
                // always check and limit the deisred Position
                if(axisStructPtr->Planning.desiredPos > axisStructPtr->Position.LimitPos)
                {
                    axisStructPtr->Planning.desiredPos = axisStructPtr->Position.LimitPos;
                }
                else if(axisStructPtr->Planning.desiredPos < axisStructPtr->Position.LimitNeg)
                {
                    axisStructPtr->Planning.desiredPos = axisStructPtr->Position.LimitNeg;
                }

                // determine change in position desired
                axisStructPtr->Planning.delataPos = axisStructPtr->Planning.desiredPos-axisStructPtr->Position.Cmd;

                // if any motion desired
                if(axisStructPtr->Planning.delataPos != 0)
                {
                    // initialize dynamic variables
                    axisStructPtr->Position.Cmd = axisStructPtr->Position.Fbk;
                    axisStructPtr->Planning.cmdVelocity = 0;
                    axisStructPtr->Planning.Tmotion = -axisStructPtr->Planning.dT;
                    // determine positive or negative motion desired
                    if(axisStructPtr->Planning.delataPos > 0)
                    {
                        axisStructPtr->Planning.desiredMotionState = motionPositive;
                    }
                    else
                    {
                        axisStructPtr->Planning.desiredMotionState = motionNegative;
                        axisStructPtr->Planning.delataPos = -axisStructPtr->Planning.delataPos;
                    }
                    // calculate critical time boundaries
                    if(axisStructPtr->Planning.delataPos>(axisStructPtr->Planning.motionVelocity*axisStructPtr->Planning.motionVelocity)/axisStructPtr->Planning.motionAcceleration)
                    {
                        // time for constant velocity
                        axisStructPtr->Planning.Tomega = axisStructPtr->Planning.delataPos/axisStructPtr->Planning.motionVelocity-axisStructPtr->Planning.motionVelocity/axisStructPtr->Planning.motionAcceleration;
                    }
                    else
                    {
                        axisStructPtr->Planning.Tomega = 0;
                        // motion velocity command limited by range and acceleration
                        axisStructPtr->Planning.motionVelocity = sqrtFloat(axisStructPtr->Planning.delataPos*axisStructPtr->Planning.motionAcceleration);
                    }
                    // time for accel / decel
                    axisStructPtr->Planning.Talpha = axisStructPtr->Planning.motionVelocity/axisStructPtr->Planning.motionAcceleration;
                    // negate the acceleration for negative motion
                    if(axisStructPtr->Planning.desiredMotionState == motionNegative)
                    {
                        axisStructPtr->Planning.motionAcceleration = -axisStructPtr->Planning.motionAcceleration;
                    }
                }
            }
            // if motion is desired
            if(axisStructPtr->Planning.desiredMotionState != motionStopped)
            {
                // increment time in motion by dT
                axisStructPtr->Planning.Tmotion += axisStructPtr->Planning.dT;
                // compare time in motion to first time boundary
                if(axisStructPtr->Planning.Tmotion<axisStructPtr->Planning.Talpha)
                {
                    // accel to motion velocity
                    axisStructPtr->Planning.cmdVelocity += axisStructPtr->Planning.motionAcceleration*axisStructPtr->Planning.dT;
                    axisStructPtr->Position.Cmd += axisStructPtr->Planning.cmdVelocity*axisStructPtr->Planning.dT;
                }
                else // a move could have trapezoidal or triangleular velocity profile depending on position delta
                {
                    if(axisStructPtr->Planning.Tomega == 0) // triangle
                    {
                        if(axisStructPtr->Planning.Tmotion<2*axisStructPtr->Planning.Talpha)
                        {
                            // decel to stop
                            axisStructPtr->Planning.cmdVelocity -= axisStructPtr->Planning.motionAcceleration*axisStructPtr->Planning.dT;
                            axisStructPtr->Position.Cmd += axisStructPtr->Planning.cmdVelocity*axisStructPtr->Planning.dT;
                        }
                        else
                        {
                            // end with command equal to desired position
                            axisStructPtr->Position.Cmd = axisStructPtr->Planning.desiredPos;
                            axisStructPtr->Planning.desiredMotionState = motionStopped;
                        }
                    }
                    else // trapezoid
                    {
                        if(axisStructPtr->Planning.Tmotion<(axisStructPtr->Planning.Talpha+axisStructPtr->Planning.Tomega))
                        {
                            // constant velocity
                            axisStructPtr->Position.Cmd += axisStructPtr->Planning.cmdVelocity*axisStructPtr->Planning.dT;
                        }
                        else if(axisStructPtr->Planning.Tmotion<(2*axisStructPtr->Planning.Talpha+axisStructPtr->Planning.Tomega))
                        {
                            // decel to stop
                            axisStructPtr->Planning.cmdVelocity -= axisStructPtr->Planning.motionAcceleration*axisStructPtr->Planning.dT;
                            axisStructPtr->Position.Cmd += axisStructPtr->Planning.cmdVelocity*axisStructPtr->Planning.dT;
                        }
                        else
                        {
                            // end with command equal to desired position
                            axisStructPtr->Position.Cmd = axisStructPtr->Planning.desiredPos;
                            axisStructPtr->Planning.desiredMotionState = motionStopped;
                        }
                    }
                }
            }
        }
        break;
        case controlVelocity:
        {

        }
        break;
        case controlCurrent:
        {

        }
        break;
        case controlPWM:
        {

        }
        break;
        default:break;
        }





    }
    else
    {
        axisStructPtr->Position.Cmd = axisStructPtr->Position.Fbk;
        axisStructPtr->Planning.desiredPos = axisStructPtr->Position.Fbk;
    }
}
void positionLoop(struct axisStruct* axisStructPtr)
{
    if(axisStructPtr->ctrlEnabled)
    {
        if(axisStructPtr->Position.Cmd > axisStructPtr->Position.LimitPos)
        {
            axisStructPtr->Position.Cmd = axisStructPtr->Position.LimitPos;
        }
        else if(axisStructPtr->Position.Cmd < axisStructPtr->Position.LimitNeg)
        {
            axisStructPtr->Position.Cmd = axisStructPtr->Position.LimitNeg;
        }

        axisStructPtr->PosController.cmdVel = (axisStructPtr->Position.Cmd - axisStructPtr->PosController.cmdLast)/axisStructPtr->PosController.dT;

        axisStructPtr->Position.Err = axisStructPtr->Position.Cmd - axisStructPtr->Position.Fbk;

        axisStructPtr->Velocity.Cmd = axisStructPtr->PosController.Kv*axisStructPtr->Position.Err + axisStructPtr->PosController.cmdVel;
    }
    else
    {
        axisStructPtr->Velocity.Cmd = 0;
    }
    axisStructPtr->PosController.cmdLast = axisStructPtr->Position.Cmd;
}
void velocityLoop(struct axisStruct* axisStructPtr)
{
    if(axisStructPtr->ctrlEnabled)
    {
        if(axisStructPtr->Velocity.Cmd > axisStructPtr->Velocity.LimitPos)
        {
            axisStructPtr->Velocity.Cmd = axisStructPtr->Velocity.LimitPos;
        }
        else if(axisStructPtr->Velocity.Cmd < axisStructPtr->Velocity.LimitNeg)
        {
            axisStructPtr->Velocity.Cmd = axisStructPtr->Velocity.LimitNeg;
        }

        if(axisStructPtr->Planning.useEstimatedVelocity)
            axisStructPtr->Velocity.Err = axisStructPtr->Velocity.Cmd - axisStructPtr->Planning.estVelocity;
        else
            axisStructPtr->Velocity.Err = axisStructPtr->Velocity.Cmd - axisStructPtr->Velocity.Fbk;

        axisStructPtr->VelController.saturated = (axisStructPtr->PWMSaturated || axisStructPtr->CurrentSaturated);
        axisStructPtr->VelController.xn = axisStructPtr->Velocity.Err;
        preparepiControllerStruct(&axisStructPtr->VelController);
        executepiControllerStruct(&axisStructPtr->VelController);

        axisStructPtr->torqueCmd = axisStructPtr->VelController.yn;
    }
    else
    {
        axisStructPtr->torqueCmd = 0;
    }
}
void currentLoop(struct axisStruct* axisStructPtr)
{
    if(axisStructPtr->ctrlEnabled)
    {
        if(axisStructPtr->currentCtrlEnabled)
        {
            axisStructPtr->Current.Cmd = axisStructPtr->torqueCmd / axisStructPtr->MotorModel.Km;

            if(axisStructPtr->Current.Cmd > axisStructPtr->Current.LimitPos)
            {
                axisStructPtr->Current.Cmd = axisStructPtr->Current.LimitPos;
                axisStructPtr->CurrentSaturated = ui8TRUE;
            }
            else if(axisStructPtr->Current.Cmd < axisStructPtr->Current.LimitNeg)
            {
                axisStructPtr->Current.Cmd = axisStructPtr->Current.LimitNeg;
                axisStructPtr->CurrentSaturated = ui8TRUE;
            }
            else
                axisStructPtr->CurrentSaturated = ui8FALSE;

            axisStructPtr->Current.Err = axisStructPtr->Current.Cmd - axisStructPtr->Current.Fbk;
            axisStructPtr->CurController.saturated = (axisStructPtr->PWMSaturated || axisStructPtr->CurrentSaturated);
            axisStructPtr->CurController.xn = axisStructPtr->Current.Err;
            preparepiControllerStruct(&axisStructPtr->CurController);
            executepiControllerStruct(&axisStructPtr->CurController);

            axisStructPtr->voltageCmd = axisStructPtr->CurController.yn;
        }
        else
        {
            axisStructPtr->voltageCmd = axisStructPtr->torqueCmd * axisStructPtr->MotorModel.R * axisStructPtr->MotorModel.Km;
        }

        axisStructPtr->PWMCmd = axisStructPtr->voltageCmd/axisStructPtr->voltageLimit;
        if(axisStructPtr->PWMCmd > axisStructPtr->PWMLimit)
        {
            axisStructPtr->PWMCmd = axisStructPtr->PWMLimit;
            axisStructPtr->PWMSaturated = ui8TRUE;
        }
        else if(axisStructPtr->PWMCmd < -axisStructPtr->PWMLimit)
        {
            axisStructPtr->PWMCmd = -axisStructPtr->PWMLimit;
            axisStructPtr->PWMSaturated = ui8TRUE;
        }
        else
            axisStructPtr->PWMSaturated = ui8FALSE;


    }
    else
        axisStructPtr->PWMCmd = 0;
}

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
        case mcsUseEstimatedVel: myArray[i] = {i,   "Use Est. Velocity",    "(yes/no)",         0,UNSIGNED_TYPE,sizeof(mystruct->Planning.actualControlMode),   &mystruct->Planning.actualControlMode}; break;
        case mcsEstimatedVelocity: myArray[i] = {i, "Estimated Velocity",   "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct->Planning.estVelocity),         &mystruct->Planning.estVelocity};       break;
        case mcsCommandVelocity: myArray[i] = { i,  "Command Velocity",     "(rad/sec)",        1,FLOAT_TYPE,   sizeof(mystruct->Planning.cmdVelocity),         &mystruct->Planning.cmdVelocity};       break;
        case mcsLastPosition: myArray[i] = { i,     "Last Position",        "(rad)",            1,FLOAT_TYPE,   sizeof(mystruct->Planning.LastFbkPosition),     &mystruct->Planning.LastFbkPosition};   break;
        case mcsPositionResolution: myArray[i] = {i,"Position Resolution",  "(rad)",            0,FLOAT_TYPE,   sizeof(mystruct->Planning.PositionResolution),  &mystruct->Planning.PositionResolution};break;
        case mcsActualControlMode: myArray[i] = {i, "Actual Control Mode",  "",                 1,SIGNED_TYPE,  sizeof(mystruct->Planning.actualControlMode),   &mystruct->Planning.actualControlMode}; break;
        case mcsDesiredControlMode: myArray[i]={i,  "Desired Control Mode", "",                 0,SIGNED_TYPE,  sizeof(mystruct->Planning.desiredControlMode),  &mystruct->Planning.desiredControlMode};break;
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
        case mcsPWMCMD: myArray[i] = { i,           "PWM Command",          "(-1 to 1)",        0,UNSIGNED_TYPE,sizeof(mystruct->CurController.reset),          &mystruct->CurController.reset};        break;
    
        
        }
        
    }
}