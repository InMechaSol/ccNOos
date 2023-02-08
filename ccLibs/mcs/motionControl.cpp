/** \file motionControl.cpp
*   \brief <a href="https://www.inmechasol.org/" target="_blank">IMS</a>:
		<a href="https://github.com/InMechaSol/ccNOos" target="_blank">ccNOos</a>,
		Implementation for C++ wrappers 

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

#include "motionControl.c"
#include "motionControlSerialize.h"

SmartMotorDevice::SmartMotorDevice(struct axisStruct* mystructIn,struct SPDStruct* AxisSPDStructArrayIn)
{
    AxisSPDStructArray = AxisSPDStructArrayIn;
    mystruct = mystructIn;
    LinkAxisSPDStructArray(mystruct, AxisSPDStructArray);
}
void SmartMotorDevice::prepare()
{

}
void SmartMotorDevice::execute()
{
    // Latch Feedback and Current Status
    if(!mystruct->ignoreEE)
    {
        mystruct->Current.Fbk = mystruct->MotorModel.I;
        mystruct->Velocity.Fbk = mystruct->MotorModel.W;
        // what about position feedback? need add to motor model
    }
    else
    {
        // no current feedback in the mode
        mystruct->Velocity.Fbk = mystruct->AxisLoadInertia.Omega;
        mystruct->Position.Fbk = mystruct->AxisLoadInertia.Theta;
    }


    if(mystruct->Reset)
    {
        mystruct->Reset = ui8FALSE;
        mystruct->MotorModel.Reset = ui8TRUE;
        mystruct->AxisLoadInertia.Reset = ui8TRUE;
        mystruct->CurController.reset = ui8TRUE;
        mystruct->VelController.reset = ui8TRUE;
        mystruct->Time = 0;
    }

    // planning loop
    planningLoop(mystruct);

    // position loop
    positionLoop(mystruct);

    // velocity loop
    velocityLoop(mystruct);

    if(!mystruct->ignoreEE)
    {
        // current loop
        currentLoop(mystruct);

        // voltage/pwm

        // dc motor model
        preparedcMotorStruct(&mystruct->MotorModel);
        int times = mystruct->CurController.dT/mystruct->MotorModel.dT;
        for(int i = 0; i < times; i++)
        {
            executedcMotorStruct(&mystruct->MotorModel);
        }
        mystruct->Time += mystruct->CurController.dT;
    }
    else
    {
        // Axis Load simulation with torque input
        mystruct->AxisLoadInertia.TorqueApplied = mystruct->torqueCmd;
        preparerotatingInertiaStruct(&mystruct->AxisLoadInertia);
        executerotatingInertiaStruct(&mystruct->AxisLoadInertia);
        mystruct->Time += mystruct->AxisLoadInertia.dT;
    }

}
struct SPDStruct* SmartMotorDevice::getSPDArray()
{
    return &AxisSPDStructArray[mcsNone];
}

