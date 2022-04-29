/** \file Platform_ccOS.hpp
*   \brief <a href="https://www.inmechasol.org/" target="_blank">IMS</a>:
        <a href="https://github.com/InMechaSol/ccNOos" target="_blank">ccNOos</a>,
        Platform Specification, ccOS

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

#if PLATFORM_NAME!=ccOS
error PLATFORM_NAME must be ccOS
#endif


#ifdef REDEFINE_NULLPTR
#error Must not compile with -DREDEFINE_NULLPTR on ccOS
#endif // !REDEFINE_NULLPTR
#ifdef __NOEXCEPTIONS
#error Must not compile with -D__NOEXCEPTIONS on ccOS
#endif // !__NOEXCEPTIONS





#include "os_execution_system.hpp"
#include "console_menu.h"
#include <cmath>
#include <ctime>
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <fstream>


// 0) (Optional) Platform Config and Log Files/Devices

// 1) Platform Setup Function
void platformSetup()
{


}
// 2) Platform Start Function
void platformStart()
{
    //<platformStart>
    //</platformStart>
}
// 3) Platform Loop Delay Function
void platformLoopDelay()
{
    //<platformLoopDelay>
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    //</platformLoopDelay>
}
#ifdef __USINGCONSOLEMENU

// 8) Platform API Functions (From Template?)
PlatformAPIFuncsTemplate(size + 1);
#endif
// 9) Global Execution System Instance
//executionSystemClass PLATFORM_EXESYS_NAME(Plat)(uSEC_PER_CLOCK);
// 10) ExeSys API Functions (From Template?)
ExeSysAPIFuncsTemplate

// 11) OS execution system instance builds on ccNOos level exeSys
OSexecutionSystemClass theExecutionSystem(uSEC_PER_CLOCK);

