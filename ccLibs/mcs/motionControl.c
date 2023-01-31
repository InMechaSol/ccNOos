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
    outStruct.period = 1.0;//sec
    outStruct.amplitude = 0.0;
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
    outStruct.motionVelocity = 0;
    outStruct.motionAcceleration = 0;
    outStruct.estVelocity = 0;
    outStruct.cmdVelocity = 0;
    outStruct.LastFbkPosition = 0;
    outStruct.PositionResolution = 0.01;
    outStruct.actualControlMode = controlPWM;
    outStruct.desiredControlMode = controlPWM;
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
    outStruct.R = 1;// 0.045; // 0.045 (ohms) IQ2306
    outStruct.L = 0.003; // 0.003 (henry) NEMA23 similar spec
    outStruct.Km = 1;// 1 / 0.0043; // 1/0.0043 (A/Nm) IQ2306
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
void commandGenerator(struct axisStruct* axisStructPtr)
{
    // detect changes and protect transitions
    if (axisStructPtr->Planning.desiredControlMode != axisStructPtr->Planning.actualControlMode)
    {
        axisStructPtr->Planning.cmdGenerator.desiredGenMode = generateNothing;
        axisStructPtr->Planning.Tmotion = -axisStructPtr->Planning.dT;
    }
    if (axisStructPtr->Planning.cmdGenerator.actualGenMode != axisStructPtr->Planning.cmdGenerator.desiredGenMode)
    {
        axisStructPtr->Planning.cmdGenerator.actualGenMode = axisStructPtr->Planning.cmdGenerator.desiredGenMode;
        axisStructPtr->Planning.Tmotion = -axisStructPtr->Planning.dT;
    }

    // increment time in motion by dT
    axisStructPtr->Planning.Tmotion += axisStructPtr->Planning.dT;

    // always output zero, unless...
    switch (axisStructPtr->Planning.cmdGenerator.actualGenMode)
    {
    case generatePulse: 
    {
        if (axisStructPtr->Planning.Tmotion < axisStructPtr->Planning.cmdGenerator.period)
        {
            axisStructPtr->Planning.cmdGenerator.cmdOutput = axisStructPtr->Planning.cmdGenerator.amplitude;
        }
        else
            axisStructPtr->Planning.cmdGenerator.cmdOutput = 0.0;
    }
    break;
    case generatePulseTrain:
    {
        float intPart, fracPart;
        fracPart = ModuloFloat((axisStructPtr->Planning.Tmotion/ axisStructPtr->Planning.cmdGenerator.period), &intPart);
        if (fracPart < axisStructPtr->Planning.cmdGenerator.dutycyle)
        {
            axisStructPtr->Planning.cmdGenerator.cmdOutput = axisStructPtr->Planning.cmdGenerator.amplitude;
        }
        else
            axisStructPtr->Planning.cmdGenerator.cmdOutput = 0.0;
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
        case controlPosition: 
        {
            // generate stream of position command setpoints to feed the position loop input

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
            // if motion is desired, generate command profile
            if(axisStructPtr->Planning.desiredMotionState != motionStopped)
            {
                // increment time in motion by dT
                axisStructPtr->Planning.Tmotion += axisStructPtr->Planning.dT;
                // compare time in motion to first time boundary
                if (axisStructPtr->Planning.Tmotion < axisStructPtr->Planning.Talpha)
                {
                    // accel to motion velocity
                    axisStructPtr->Planning.cmdVelocity += axisStructPtr->Planning.motionAcceleration * axisStructPtr->Planning.dT;
                    axisStructPtr->Position.Cmd += axisStructPtr->Planning.cmdVelocity * axisStructPtr->Planning.dT;
                }
                else // a move could have trapezoidal or triangleular velocity profile depending on position delta
                {
                    if (axisStructPtr->Planning.Tomega == 0) // triangle
                    {
                        if (axisStructPtr->Planning.Tmotion < 2 * axisStructPtr->Planning.Talpha)
                        {
                            // decel to stop
                            axisStructPtr->Planning.cmdVelocity -= axisStructPtr->Planning.motionAcceleration * axisStructPtr->Planning.dT;
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
                            axisStructPtr->Planning.cmdVelocity -= axisStructPtr->Planning.motionAcceleration * axisStructPtr->Planning.dT;
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
        case controlVelocity:
        {
            // generate stream of velocity command setpoints to feed the velocity loop input
            commandGenerator(axisStructPtr);
            axisStructPtr->Velocity.Cmd = axisStructPtr->Planning.cmdGenerator.cmdOutput;
        }
        break;
        case controlCurrent:
        {
            // generate stream of current command setpoints to feed the current loop input
            commandGenerator(axisStructPtr);
            axisStructPtr->Current.Cmd = axisStructPtr->Planning.cmdGenerator.cmdOutput;
        }
        break;
        case controlPWM:
        {
            // generate stream of pwm command setpoints to directly drive motor throttle
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
            if (axisStructPtr->currentCtrlEnabled)
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
            

            if (axisStructPtr->Current.Fbk > axisStructPtr->Current.LimitPos)
            {
                //axisStructPtr->PWMCmd = ;
                axisStructPtr->PWMSaturated = ui8TRUE;
            }
            else if (axisStructPtr->Current.Fbk < axisStructPtr->Current.LimitNeg)
            {
                //axisStructPtr->PWMCmd = ;
                axisStructPtr->PWMSaturated = ui8TRUE;
            }
            else
                axisStructPtr->PWMSaturated = ui8FALSE;

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
