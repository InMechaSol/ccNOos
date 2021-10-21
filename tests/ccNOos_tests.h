/** \file ccNOos_tests.h
*   \brief Cross-Platform Portable ccNOos Tests Declarations

   Copyright 2021 InMechaSol, Inc

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

Notes:

This header demonstrates the usage of the ccNOos library and its dependence
on other files in the library.  A platform specific main(.c,.cpp) file need
only include this single header to implement a platform specific ccNOos_tests
application.

*/
#ifndef __CCNOOS_TESTS__
#define __CCNOOS_TESTS__  
    
#include "../executionSystem/execution_system.h"    // -> includes version_config.h
#include "../consoleMenu/console_menu.h"            // -> includes api_comp_mod.h -> compute_module.h -> version_config.h
#include "../deviceCompModule/dev_comp_mod.h"       // -> includes io_device.h -> version_config.h
    
#ifdef __cplusplus
extern "C" {    
#endif // !__cplusplus

///////////////////////////////////////////////////////////////////////
// SysTick Example - Device Module Configuration
///////////////////////////////////////////////////////////////////////
#define MIN_LED_INDEX (0u)
#define SEC_LED_INDEX (1u)
#define TIME_SERIAL_INDEX (2u)
#define SYSTICK_EXAMPLE_DEVCOUNT (3u)
struct SysTickExampleDevModStruct
{
    struct deviceCompModStruct devMod;
    struct ioDeviceStruct devArray[SYSTICK_EXAMPLE_DEVCOUNT];
    // Device Compute Module Data
    uint32_t secCount_Last, secCount;
    uint32_t minCount_Last, minCount, hrCount;
    uint8_t MinLEDvalue, SecLEDvalue, Light_Off;    
    char time[16u];

};
struct SysTickExampleDevModStruct CreateSysTickExampleDevModStruct(
    int (*writeMinLED)(),
    int (*writeSecLED)(),
    int (*writeSerialTime)(),
    struct executionSystemStruct* exeSysPtrIn
);

// Re-usable, portable, cross-platform (systick example setup() function)
int setup_systickExample(void *dataPtr);

// Re-usable, portable, cross-platform (systick example setup() function)
int loop_systickExample(void *dataPtr);


#ifdef __cplusplus
}    
#endif // !__cplusplus
#endif // !__CCNOOS_TESTS__