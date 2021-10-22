/** \file ccNOos_tests.c
*   \brief Cross-Platform Portable ccNOos Tests Definitions

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

This source demonstrates the usage of the ccNOos library and its dependence
on other files in the library.  A platform specific main(.c,.cpp) file need
only instantiate, configure, then execute the execuion system.  

*/
#include "ccNOos_tests.h"

///////////////////////////////////////////////////////////////////////
// SysTick Example - Device Module Configuration
///////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <math.h>

// Re-usable, portable, cross-platform (systick example create() function)
struct SysTickExampleDevModStruct CreateSysTickExampleDevModStruct(
    int (*writeMinLED)(),
    int (*writeSecLED)(),
    int (*writeSerialTime)(),
    struct executionSystemStruct* exeSysPtrIn,
    int lightOff
)
{
    struct SysTickExampleDevModStruct outStruct;
    
    outStruct.devArray[SEC_LED_INDEX] = createIODeviceStruct(0u, 0u, 0u, writeSecLED );
    outStruct.devArray[MIN_LED_INDEX] = createIODeviceStruct(0u, 0u, 0u, writeMinLED );
    outStruct.devArray[TIME_SERIAL_INDEX] = createIODeviceStruct(0u, 0u, 0u, writeSerialTime );
    
    outStruct.devMod = CreateDevCompModStruct(exeSysPtrIn);
    
    outStruct.Light_Off = lightOff;

    return outStruct;
}

// Re-usable, portable, cross-platform (systick example setup() function)
int setup_systickExample(void *dataPtr)
{
    struct SysTickExampleDevModStruct* sysTickData = ((struct SysTickExampleDevModStruct*)(dataPtr));
    if(!sysTickData)
        return RETURN_ERROR;
    
    sysTickData->secCount_Last = 0u;
    sysTickData->secCount = 0u;
    sysTickData->hrCount = 0u;
    sysTickData->minCount_Last = 0u;
    sysTickData->minCount = 0u;

    /* Set initial state (off) for LED */
    sysTickData->MinLEDvalue = sysTickData->Light_Off;
    sysTickData->SecLEDvalue = sysTickData->Light_Off;
    
    if(sysTickData->devArray[MIN_LED_INDEX].write)
      sysTickData->devArray[MIN_LED_INDEX].write();
    if(sysTickData->devArray[SEC_LED_INDEX].write)
        sysTickData->devArray[SEC_LED_INDEX].write(); 
            
    return RETURN_SUCCESS;
}

// Re-usable, portable, cross-platform (systick example loop() function)
int loop_systickExample(void *dataPtr)
{
    struct SysTickExampleDevModStruct* sysTickData = ((struct SysTickExampleDevModStruct*)(dataPtr));
    if(!sysTickData)
        return RETURN_ERROR;
    
    sysTickData->secCount = getuSecTicks(sysTickData->devMod.compMod.exeSysPtr) / TIME_uS_PER_SEC;
    sysTickData->hrCount = getHourTicks(sysTickData->devMod.compMod.exeSysPtr);
    
    if(sysTickData->secCount_Last != sysTickData->secCount)
    {
        sysTickData->secCount_Last = sysTickData->secCount;
        sysTickData->minCount = (sysTickData->secCount / TIME_SEC_PER_MIN) % TIME_MIN_PER_HR;
        if(sysTickData->minCount != sysTickData->minCount_Last)
        {
            sysTickData->minCount_Last = sysTickData->minCount;
            /* Toggle red LED. 1 time per minute*/
            if(sysTickData->MinLEDvalue == sysTickData->Light_Off)
                sysTickData->MinLEDvalue = !sysTickData->Light_Off;
            else
                sysTickData->MinLEDvalue = sysTickData->Light_Off;
            if(sysTickData->devArray[MIN_LED_INDEX].write)
                sysTickData->devArray[MIN_LED_INDEX].write();
        }        
        
        /* Toggle blue LED. 1 time per second */
        if(sysTickData->SecLEDvalue == sysTickData->Light_Off)
            sysTickData->SecLEDvalue = !sysTickData->Light_Off;
        else
            sysTickData->SecLEDvalue = sysTickData->Light_Off;
        if(sysTickData->devArray[SEC_LED_INDEX].write)
            sysTickData->devArray[SEC_LED_INDEX].write();
        
        /* Print system time to IO Device every second */
        sprintf(sysTickData->time, "\r%02u:%02u:%02u", 
                            (int)(sysTickData->hrCount % 100), 
                            (int)sysTickData->minCount,
                            (int)((sysTickData->secCount) % TIME_SEC_PER_MIN)
                            );
        if(sysTickData->devArray[TIME_SERIAL_INDEX].write)
            sysTickData->devArray[TIME_SERIAL_INDEX].write();
    }
    return RETURN_SUCCESS;
}


