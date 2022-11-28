/** \file ConsoleMenuAPI.h
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
#ifndef __GRIPPERCONSOLEAPI_H__
#define __GRIPPERCONSOLEAPI_H__  

#include "Application_Solution.h"





const char* gripperModeString(enum enumGripperMode GripperModeIn);
const char* vacuumOnOffString(UI_8 vacCmd);
const char* cylinderExtendRetractString(UI_8 extendCmd);
const char* motorStatusString(struct smartMotorStruct* smartMotorStructPtrIn);



#ifdef __cplusplus

class gripperFW_menuNode :public menuNode
{
protected:
    ccGripper_class* ccgripperptr = nullptr;
public:
    gripperFW_menuNode(ccGripper_class* ccgripperptrIn);

};

#endif // !__cplusplus
#endif // !__GRIPPERCONSOLEAPI_H__
