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

#include "motionControlSerialize.c"

///////////////////////////////////////////////////////////////////////////
//  AxisSPD
// - to be moved to the serialization layer?
//
enum mcsSPDSelector AxisSPD::getSPDSelector()
{
    return AxisVarSelection;
}
SmartMotorDevice* AxisSPD::getSMDevPtr()
{
    return smDevPtr;
}
AxisSPD::AxisSPD(enum mcsSPDSelector AxisVarSelectionIn, SmartMotorDevice* smDevPtrIn):
    SPDClass(AxisVarSelectionIn, smDevPtrIn, smDevPtrIn->getSPDArray())
{
    AxisVarSelection = AxisVarSelectionIn;
    smDevPtr = smDevPtrIn;
}

float AxisSPD::getFloatVal()
{
    return getSPDFloatValue(AxisVarSelection, smDevPtr->getSPDArray());
}
const char* AxisSPD::getLabelString()
{
    return getSPDLabelString(AxisVarSelection, smDevPtr->getSPDArray());
}
const char* AxisSPD::getUnitsString()
{
    return getSPDUnitsString(AxisVarSelection, smDevPtr->getSPDArray());
}
