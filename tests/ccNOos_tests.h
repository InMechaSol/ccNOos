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
    
#include "../executionSystem/execution_system.h"    
#include "../consoleMenu/console_menu.h"            
    
///////////////////////////////////////////////////////////////////////
// SysTick Example - Device Module Configuration
///////////////////////////////////////////////////////////////////////
#define TIME_STR_LEN (16u)
#define MIN_LED_INDEX (0u)
#define SEC_LED_INDEX (1u)
#define TIME_SERIAL_INDEX (2u)

struct SysTickStruct
{
    struct computeModuleStruct compMod;
    uint32_t secCount_Last, secCount, minCount_Last, minCount, hrCount;
    uint8_t MinLEDvalue, SecLEDvalue, Light_Off;    
    char time[TIME_STR_LEN];    
};

struct SysTickStruct CreateSysTickStruct(
    int lightOff
);

// platform and application specific time string serialization
void SerializeTimeString(struct SysTickStruct* sysTickDataPtr);

// platform and application specific io device functions
void WriteMinLED(struct SysTickStruct* sysTickDataPtr);
void WriteSecLED(struct SysTickStruct* sysTickDataPtr);
void WriteTimeSerial(struct SysTickStruct* sysTickDataPtr);

// Re-usable, portable, cross-platform (systick example setup() function)
int setup_systickExample(struct computeModuleStruct* compModPtr);

// Re-usable, portable, cross-platform (systick example setup() function)
int loop_systickExample(struct computeModuleStruct* compModPtr);


#ifdef __cplusplus
////////////////////////////////////////////////////////////////////////////////
// C++ SysTick Example Class - built from computeModuleClass
class SysTickExample_class : public computeModuleClass
{
private:
    struct SysTickStruct SysTickData;
public:
    SysTickExample_class(
        int lightOff
        );
    int mod_setup()
    {
        return setup_systickExample(
                (struct computeModuleStruct*)(&SysTickData)
                );
    }
    int mod_loop() 
    {
        return loop_systickExample(
                (struct computeModuleStruct*)(&SysTickData)
                );
    }
    void mod_systick() {;} // do nothing in systic module exe area
    int mod_excphandler()
    {
        return setup_systickExample(
                (struct computeModuleStruct*)(&SysTickData)
                );
    }
};

#endif // !__cplusplus
#endif // !__CCNOOS_TESTS__