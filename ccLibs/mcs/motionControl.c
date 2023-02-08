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



struct generatorStruct creategeneratorStruct()
{
    struct generatorStruct outStruct;
    outStruct.actualGenMode = generateNothing;
    outStruct.desiredGenMode = generateNothing;
    outStruct.dutycyle = 0.5;  //0-1
    outStruct.period = 10.0;//sec
    outStruct.offset = -1.0;
    outStruct.amplitude = 2.0;
    outStruct.cmdOutput = 0.0;
    return outStruct;
}



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
    outStruct.desiredVelocity = 5;
    outStruct.desiredAcceleration = 1;
    outStruct.estVelocity = 0;
    outStruct.cmdVelocity = 0;
    outStruct.LastFbkPosition = 0;
    outStruct.PositionResolution = 0.01;
    outStruct.actualControlMode = controlPosition;
    outStruct.desiredControlMode = controlPosition;
    outStruct.actualMotionState = motionStopped;
    outStruct.desiredMotionState = motionStopped;
    outStruct.useEstimatedVelocity = ui8FALSE;
    outStruct.cmdGenerator = creategeneratorStruct();
    return outStruct;
}
///
/// \brief createposConrolStruct
/// \return
///
struct posControlStruct createposConrolStruct()
{
    struct posControlStruct outStruct;
    outStruct.Kv = 10;
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
    outStruct.Kp = 10;
    outStruct.Ti = 1;
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
/// \brief createrotatingInertiaStruct
/// \return rotatingInertiaStruct
///
struct rotatingInertiaStruct createrotatingInertiaStruct()
{
    struct rotatingInertiaStruct outStruct;
    outStruct.dT = 0.001;
    outStruct.Inertia = 1;
    outStruct.Efficiency = 0.97;
    outStruct.Rf = 0.15;
    outStruct.Kf = 0;
    outStruct.TorqueRating = 1;
    outStruct.Omega = 0;
    outStruct.OmegaDot = 0;
    outStruct.Theta = 0;
    outStruct.ThetaDot = 0;
    outStruct.TorqueApplied = 0;
    outStruct.TorqueFriction = 0;
    outStruct.Reset = ui8FALSE;
    outStruct.WminResolution = 0.001;
    outStruct.AminResolution = 0.001;
    return outStruct;
}
#define P rotatingInertiaStructPtrIn
void preparerotatingInertiaStruct(struct rotatingInertiaStruct* P)
{
    P->Kf = P->TorqueRating*(1-P->Efficiency);
}
void executerotatingInertiaStruct(struct rotatingInertiaStruct* P)
{
    // handle reset
    if(P->Reset)
    {
        P->ThetaDot = 0;
        P->OmegaDot = 0;
        P->Theta = 0;
        P->Omega = 0;
        P->Reset = ui8FALSE;
    }

    // calculate friction estimate from states and efficiency parameter
    if(P->EstFriction)
    {
        if( (P->Omega <= P->WminResolution && P->Omega>= -P->WminResolution)&&
            (P->OmegaDot <= P->AminResolution && P->OmegaDot>= -P->AminResolution)
                )
        {
            // Static Friction Always oppose attempted Motion
            P->TorqueFriction = -P->TorqueApplied;

            // Static Friction Cut-off Torque
            if(P->TorqueFriction < -P->Kf)
                P->TorqueFriction = -P->Kf;
            else if(P->TorqueFriction > P->Kf)
                P->TorqueFriction = P->Kf;
        }
        else
        {
            // Kinetic Friction Always oppose actual Motion
            if(P->Omega > P->WminResolution)
            {
                P->TorqueFriction = -P->Rf*P->Kf;
            }
            else if(P->Omega < -P->WminResolution)
            {
                P->TorqueFriction = P->Rf*P->Kf;
            }
        }
    }
    else
    {
        P->TorqueFriction = 0.0;
    }

    // calculate the SS model differentials
    P->OmegaDot = (P->TorqueApplied + P->TorqueFriction) / P->Inertia;
    P->ThetaDot = P->Omega;

    // integrate the ss model
    P->Omega += P->OmegaDot*P->dT;
    P->Theta += P->ThetaDot*P->dT;
}
#undef P

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
    outStruct.R = 1;// 0.045; // 0.045 (ohms) IQ2306
    outStruct.L = 0.003; // 0.003 (henry) NEMA23 similar spec
    outStruct.Km = 1;// 1 / 0.0043; // 1/0.0043 (A/Nm) IQ2306
    outStruct.WminResolution = 0.0001; // 0.0001 (rad/sec)
    outStruct.Vcc = 24; // 24 (V) line voltage to motor pwm driver

    outStruct.Reset = ui8TRUE;

    // Calculate Coefficients
    preparedcMotorStruct(&outStruct);

    return outStruct;
}
#define P dcMotorStructPtrIn
void preparedcMotorStruct(struct dcMotorStruct* P)
{
    // calculate ss coefficiencts from motor parameters
    P->a11 = -P->R/P->L;
    P->a12 = -P->Km/P->L;
    P->a21 = 1/(P->Km*P->J);
    P->a22 = 0;
}
void executedcMotorStruct(struct dcMotorStruct* P)
{
    if(P->Reset)
    {
        P->Idot = 0;
        P->Wdot = 0;
        P->I = 0;
        P->W = 0;
        P->Reset = ui8FALSE;
    }

    // calculate friction estimate from states and efficiency parameter
    if(P->W <= P->WminResolution && P->W >= -P->WminResolution)
    {
        // Static Friction Always oppose Motion
        if(P->I>0)
            P->FrictionTorque = -(P->a21*P->I + P->a22*P->W)*P->J;
        else if(P->I<0)
            P->FrictionTorque = (P->a21*P->I + P->a22*P->W)*P->J;
        else
            P->FrictionTorque = 0.0;

        // Static Friction Cut-off Torque
        if(P->FrictionTorque < -P->ContinousTorque*(1-P->Efficiency))
            P->FrictionTorque = -P->ContinousTorque*(1-P->Efficiency);
        else if(P->FrictionTorque > P->ContinousTorque*(1-P->Efficiency))
            P->FrictionTorque = P->ContinousTorque*(1-P->Efficiency);
    }
    else
    {
        // Kinetic Friction
        if(P->W > P->WminResolution)
        {
            P->FrictionTorque = -.25*P->ContinousTorque*(1-P->Efficiency);
        }
        else if(P->W < -P->WminResolution)
        {
            P->FrictionTorque = .25*P->ContinousTorque*(1-P->Efficiency);
        }
    }
    P->FrictionTorque = 0.0;

    // calculate ss differentials from states, coefficients, and inputs
    P->Idot = P->a11*P->I + P->a12*P->W + P->V*1/P->L;
    P->Wdot = P->a21*P->I + P->a22*P->W + P->FrictionTorque*1/P->J;

    // calculate ss next states from ss differentials and time delta
    P->I += P->Idot*P->dT;
    P->W += P->Wdot*P->dT;
    //P->Time += P->dT;  // see @void SmartMotorDevice::execute()

    // calculate observed variables from states and parameters
    P->Torque = P->I/P->Km;
    P->Vemf = P->Km*P->W;
}
#undef P

///
/// \brief createaxisStruct
/// \return
///
struct axisStruct createaxisStruct()
{
    struct axisStruct outStruct;
    outStruct.Planning = createplanningStruct();
    outStruct.Position = createcontroledFloat();
    outStruct.Position.LimitNeg = -100;
    outStruct.Position.LimitPos = 100;
    outStruct.PosController = createposConrolStruct();
    outStruct.Velocity = createcontroledFloat();
    outStruct.Velocity.LimitNeg = -10;
    outStruct.Velocity.LimitPos = 10;
    outStruct.VelController = createpiControllerStruct();
    outStruct.Current = createcontroledFloat();    
    outStruct.MotorModel =  createdcMotorStruct();
    outStruct.CurController = createpiControllerStruct();
    outStruct.torqueCmd = 0;
    outStruct.voltageCmd = 0;
    outStruct.voltageLimit = 1;
    outStruct.PWMCmd = 0;
    outStruct.PWMLimit = 0;
    outStruct.PWMSaturated = 0;
    outStruct.PWMCmdSafe = 0;
    outStruct.currentCtrlEnabled = 0;
    outStruct.CurrentSaturated = 0;
    outStruct.ctrlEnabled = 0;
    outStruct.AxisLoadInertia = createrotatingInertiaStruct();
    outStruct.ignoreEE = ui8TRUE;
    return outStruct;
};

///
/// \brief Cascade Control Loops
///
void commandGenerator(struct axisStruct* axisStructPtr)
{
    // detect changes and protect transitions
    if (axisStructPtr->Planning.desiredControlMode != axisStructPtr->Planning.actualControlMode)
    {
        axisStructPtr->Planning.cmdGenerator.desiredGenMode = generateNothing;
        axisStructPtr->Planning.cmdGenerator.genTime = -axisStructPtr->Planning.dT;
    }
    if (axisStructPtr->Planning.cmdGenerator.actualGenMode != axisStructPtr->Planning.cmdGenerator.desiredGenMode)
    {
        axisStructPtr->Planning.cmdGenerator.actualGenMode = axisStructPtr->Planning.cmdGenerator.desiredGenMode;
        axisStructPtr->Planning.cmdGenerator.genTime = -axisStructPtr->Planning.dT;
    }

    // increment generator time by dT
    axisStructPtr->Planning.cmdGenerator.genTime += axisStructPtr->Planning.dT;

    // always output zero, unless...
    switch (axisStructPtr->Planning.cmdGenerator.actualGenMode)
    {
    case generatePulse: 
    {
        if (axisStructPtr->Planning.cmdGenerator.genTime < axisStructPtr->Planning.cmdGenerator.period)
        {
            axisStructPtr->Planning.cmdGenerator.cmdOutput = axisStructPtr->Planning.cmdGenerator.amplitude;
        }
        else
            axisStructPtr->Planning.cmdGenerator.cmdOutput = 0.0;
        axisStructPtr->Planning.cmdGenerator.cmdOutput += axisStructPtr->Planning.cmdGenerator.offset;
    }
    break;
    case generatePulseTrain:
    {
        float intPart, fracPart;
        fracPart = ModuloFloat((axisStructPtr->Planning.cmdGenerator.genTime/ axisStructPtr->Planning.cmdGenerator.period), &intPart);
        if (fracPart < axisStructPtr->Planning.cmdGenerator.dutycyle)
        {
            axisStructPtr->Planning.cmdGenerator.cmdOutput = axisStructPtr->Planning.cmdGenerator.amplitude;
        }
        else
            axisStructPtr->Planning.cmdGenerator.cmdOutput = 0.0;
        axisStructPtr->Planning.cmdGenerator.cmdOutput += axisStructPtr->Planning.cmdGenerator.offset;
    }
    break;
    case generateSinWave:
    {
        axisStructPtr->Planning.cmdGenerator.cmdOutput = 0.0;
    }
    break;
    case generateNothing:
    default:
    {
        axisStructPtr->Planning.cmdGenerator.cmdOutput = 0.0;
    }
    }
    
    
}
void planningLoop(struct axisStruct* axisStructPtr)
{
    // always monitor feedback to determine actual motion state
    if((axisStructPtr->Position.Fbk-axisStructPtr->Planning.LastFbkPosition) < -axisStructPtr->Planning.PositionResolution)
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
        case controlPosition:// generate stream of position command setpoints to feed the position loop input
        {
            // use command generator to set desired position setpoint, if desired (for tuning mostly)
            commandGenerator(axisStructPtr);
            if(axisStructPtr->Planning.cmdGenerator.actualGenMode != generateNothing)
            {
                axisStructPtr->Planning.desiredPos = axisStructPtr->Planning.cmdGenerator.cmdOutput;
            }

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
                if(axisStructPtr->Planning.desiredPosLast != axisStructPtr->Planning.desiredPos)
                {
                    axisStructPtr->Planning.delataPos = axisStructPtr->Planning.desiredPos-axisStructPtr->Position.Fbk;
                    axisStructPtr->Planning.desiredPosLast = axisStructPtr->Planning.desiredPos;
                }
                else
                {
                    axisStructPtr->Planning.delataPos = 0;
                }

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
                    if(axisStructPtr->Planning.delataPos>(axisStructPtr->Planning.desiredVelocity*axisStructPtr->Planning.desiredVelocity)/axisStructPtr->Planning.desiredAcceleration)
                    {
                        // time for constant velocity
                        axisStructPtr->Planning.topMotionVelocity = axisStructPtr->Planning.desiredVelocity;
                        axisStructPtr->Planning.Tomega = axisStructPtr->Planning.delataPos/axisStructPtr->Planning.topMotionVelocity-axisStructPtr->Planning.topMotionVelocity/axisStructPtr->Planning.desiredAcceleration;
                    }
                    else
                    {
                        axisStructPtr->Planning.Tomega = 0;
                        // motion velocity command limited by range and acceleration
                        axisStructPtr->Planning.topMotionVelocity = sqrtFloat(axisStructPtr->Planning.delataPos*axisStructPtr->Planning.desiredAcceleration);
                    }




                    // time for accel / decel
                    axisStructPtr->Planning.Talpha = axisStructPtr->Planning.topMotionVelocity/axisStructPtr->Planning.desiredAcceleration;





                    // negate the acceleration for negative motion
//                    if(axisStructPtr->Planning.desiredMotionState == (UI_16)motionNegative)
//                    {
//                        axisStructPtr->Planning.motionAcceleration = -axisStructPtr->Planning.motionAcceleration;
//                    }
                }
            }

            // if motion is desired, generate command profile
            if(axisStructPtr->Planning.desiredMotionState != motionStopped)
            {
                // increment time in motion by dT
                axisStructPtr->Planning.Tmotion += axisStructPtr->Planning.dT;
                // compare time in motion to first time boundary
                if (axisStructPtr->Planning.Tmotion < axisStructPtr->Planning.Talpha)
                {
                    // accel to motion velocity
                    if(axisStructPtr->Planning.desiredMotionState == (UI_16)motionNegative)
                        axisStructPtr->Planning.cmdVelocity -= axisStructPtr->Planning.desiredAcceleration * axisStructPtr->Planning.dT;
                    else
                        axisStructPtr->Planning.cmdVelocity += axisStructPtr->Planning.desiredAcceleration * axisStructPtr->Planning.dT;
                    axisStructPtr->Position.Cmd += axisStructPtr->Planning.cmdVelocity * axisStructPtr->Planning.dT;

                }
                else // a move could have trapezoidal or triangleular velocity profile depending on position delta
                {
                    if (axisStructPtr->Planning.Tomega == 0) // triangle
                    {
                        if (axisStructPtr->Planning.Tmotion < 2 * axisStructPtr->Planning.Talpha)
                        {
                            // decel to stop
                            if(axisStructPtr->Planning.desiredMotionState == (UI_16)motionNegative)
                                axisStructPtr->Planning.cmdVelocity += axisStructPtr->Planning.desiredAcceleration * axisStructPtr->Planning.dT;
                            else
                                axisStructPtr->Planning.cmdVelocity -= axisStructPtr->Planning.desiredAcceleration * axisStructPtr->Planning.dT;
                            axisStructPtr->Position.Cmd += axisStructPtr->Planning.cmdVelocity * axisStructPtr->Planning.dT;

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
                        if (axisStructPtr->Planning.Tmotion < (axisStructPtr->Planning.Talpha + axisStructPtr->Planning.Tomega))
                        {
                            // constant velocity
                            axisStructPtr->Position.Cmd += axisStructPtr->Planning.cmdVelocity * axisStructPtr->Planning.dT;
                        }
                        else if (axisStructPtr->Planning.Tmotion < (2 * axisStructPtr->Planning.Talpha + axisStructPtr->Planning.Tomega))
                        {
                            // decel to stop
                            if(axisStructPtr->Planning.desiredMotionState == (UI_16)motionNegative)
                                axisStructPtr->Planning.cmdVelocity += axisStructPtr->Planning.desiredAcceleration * axisStructPtr->Planning.dT;
                            else
                                axisStructPtr->Planning.cmdVelocity -= axisStructPtr->Planning.desiredAcceleration * axisStructPtr->Planning.dT;
                            axisStructPtr->Position.Cmd += axisStructPtr->Planning.cmdVelocity * axisStructPtr->Planning.dT;
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
        case controlVelocity:// generate stream of velocity command setpoints to feed the velocity loop input
        {            
            commandGenerator(axisStructPtr);
            axisStructPtr->Velocity.Cmd = axisStructPtr->Planning.cmdGenerator.cmdOutput;
        }
        break;
        case controlCurrent:// generate stream of current command setpoints to feed the current loop input
        {            
            commandGenerator(axisStructPtr);
            axisStructPtr->Current.Cmd = axisStructPtr->Planning.cmdGenerator.cmdOutput;
        }
        break;
        case controlPWM:// generate stream of pwm command setpoints to directly drive motor throttle
        {            
            commandGenerator(axisStructPtr);
            axisStructPtr->PWMCmd = axisStructPtr->Planning.cmdGenerator.cmdOutput;
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
    axisStructPtr->Planning.actualControlMode = axisStructPtr->Planning.desiredControlMode;
}
void positionLoop(struct axisStruct* axisStructPtr)
{
    if(axisStructPtr->ctrlEnabled)
    {
        switch (axisStructPtr->Planning.actualControlMode) {
        case controlPosition:
        {
            if (axisStructPtr->Position.Cmd > axisStructPtr->Position.LimitPos)
            {
                axisStructPtr->Position.Cmd = axisStructPtr->Position.LimitPos;
            }
            else if (axisStructPtr->Position.Cmd < axisStructPtr->Position.LimitNeg)
            {
                axisStructPtr->Position.Cmd = axisStructPtr->Position.LimitNeg;
            }

            axisStructPtr->PosController.cmdVel = (axisStructPtr->Position.Cmd - axisStructPtr->PosController.cmdLast) / axisStructPtr->PosController.dT;

            axisStructPtr->Position.Err = axisStructPtr->Position.Cmd - axisStructPtr->Position.Fbk;

            axisStructPtr->Velocity.Cmd = axisStructPtr->PosController.Kv * axisStructPtr->Position.Err + axisStructPtr->PosController.cmdVel;
        }
        break;
        default:break;
        }
        
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
        switch (axisStructPtr->Planning.actualControlMode) {
        case controlPosition:
        case controlVelocity:
        {
            if (axisStructPtr->Velocity.Cmd > axisStructPtr->Velocity.LimitPos)
            {
                axisStructPtr->Velocity.Cmd = axisStructPtr->Velocity.LimitPos;
            }
            else if (axisStructPtr->Velocity.Cmd < axisStructPtr->Velocity.LimitNeg)
            {
                axisStructPtr->Velocity.Cmd = axisStructPtr->Velocity.LimitNeg;
            }

            if (axisStructPtr->Planning.useEstimatedVelocity)
                axisStructPtr->Velocity.Err = axisStructPtr->Velocity.Cmd - axisStructPtr->Planning.estVelocity;
            else
                axisStructPtr->Velocity.Err = axisStructPtr->Velocity.Cmd - axisStructPtr->Velocity.Fbk;

            axisStructPtr->VelController.saturated = (axisStructPtr->PWMSaturated || axisStructPtr->CurrentSaturated);
            axisStructPtr->VelController.xn = axisStructPtr->Velocity.Err;
            preparepiControllerStruct(&axisStructPtr->VelController);
            executepiControllerStruct(&axisStructPtr->VelController);

            axisStructPtr->torqueCmd = axisStructPtr->VelController.yn;
        }
        break;      
        default:break;
        }
        
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
        switch (axisStructPtr->Planning.actualControlMode) {
        case controlPosition:
        case controlVelocity:
        {
            if (axisStructPtr->currentCtrlEnabled)
            {
                axisStructPtr->Current.Cmd = axisStructPtr->torqueCmd / axisStructPtr->MotorModel.Km;
            }            

        }// intentional fall-through
        case controlCurrent:
        {
            if (axisStructPtr->Current.Cmd > axisStructPtr->Current.LimitPos)
            {
                axisStructPtr->Current.Cmd = axisStructPtr->Current.LimitPos;
                axisStructPtr->CurrentSaturated = ui8TRUE;
            }
            else if (axisStructPtr->Current.Cmd < axisStructPtr->Current.LimitNeg)
            {
                axisStructPtr->Current.Cmd = axisStructPtr->Current.LimitNeg;
                axisStructPtr->CurrentSaturated = ui8TRUE;
            }
            else
                axisStructPtr->CurrentSaturated = ui8FALSE;

            if (axisStructPtr->currentCtrlEnabled)
            {
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

            axisStructPtr->PWMCmd = axisStructPtr->voltageCmd / axisStructPtr->voltageLimit;
            
        }// intentional fall-through        
        case controlPWM:
        {
            if( (axisStructPtr->Current.Fbk > axisStructPtr->Current.LimitPos) ||
                (axisStructPtr->Current.Fbk < axisStructPtr->Current.LimitNeg))
            {
                axisStructPtr->PWMCmd = axisStructPtr->PWMCmdSafe;
                axisStructPtr->PWMSaturated = ui8TRUE;
                axisStructPtr->PWMCmdSafe = 0.9 * axisStructPtr->PWMCmdSafe;
            }
            else
            {
                axisStructPtr->PWMSaturated = ui8FALSE;
                axisStructPtr->PWMCmdSafe = axisStructPtr->PWMCmd;
            }

            // protect against NAN
            if (axisStructPtr->PWMCmd != axisStructPtr->PWMCmd)
            {
                axisStructPtr->PWMCmd = 0.0;
                axisStructPtr->PWMSaturated = ui8TRUE;
            }
            if (axisStructPtr->PWMCmd > axisStructPtr->PWMLimit)
            {
                axisStructPtr->PWMCmd = axisStructPtr->PWMLimit;
                axisStructPtr->PWMSaturated = ui8TRUE;
            }
            else if (axisStructPtr->PWMCmd < -axisStructPtr->PWMLimit)
            {
                axisStructPtr->PWMCmd = -axisStructPtr->PWMLimit;
                axisStructPtr->PWMSaturated = ui8TRUE;
            }
            
        }
        break;
        default:break;
        }

    }
    else
        axisStructPtr->PWMCmd = 0;
}
