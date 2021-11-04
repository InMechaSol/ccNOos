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
only instantiate, configure, then execute the execution system.  

*/
#include "ccNOos_tests.h"



///////////////////////////////////////////////////////////////////////
// SysTick Example
///////////////////////////////////////////////////////////////////////
#ifdef EXAMPLE_SYSTICK

// Re-usable, portable, cross-platform (systick example create() function)
//struct SysTickStruct SysTick_create( MODSTRUCTCREATEINS )
MODSTRUCT_CREATE_PROTO(MODULENAME)
{
    uint8_t i = 0;
    MODSTRUCT(MODULENAME) outStruct;
    outStruct.compMod = CreateComputeModuleStruct();    
    outStruct.Light_Off = lightOff;
    outStruct.MinLEDvalue = lightOff;
    outStruct.SecLEDvalue = lightOff;
    for(i=0; i < TIME_STR_LEN; i++)
        outStruct.time[i] = 0x00;
    outStruct.secCount = 0u;
    outStruct.secCount_Last = 0u;
    outStruct.minCount = 0u;
    outStruct.minCount_Last = 0u;
    outStruct.hrCount = 0u;
    return outStruct;
}



// Re-usable, portable, cross-platform (systick example setup() function)
MODULE_FUNC_PROTO_SETUP(MODULENAME)
{
    MODDATAPTR_ERROR_RETURN(MODULENAME, sysTickPtr);
    
    // Setup is running in the loop area to handle exceptions...
    IF_MODULE_ERROR(sysTickPtr)
    {
        CLEAR_MODULE_ERRORS(sysTickPtr);  // do nothing, clear flags
    }
    // Setup is running in the setup area following power on
    else
    {
        // Values are initialized at instantiation
        // need to write to LEDs        
        WriteMinLED(sysTickPtr);        
        WriteSecLED(sysTickPtr);
    }
    return RETURN_SUCCESS;
}

// Re-usable, portable, cross-platform (systick example loop() function)
MODULE_FUNC_PROTO_LOOP(MODULENAME)
{
    MODDATAPTR_ERROR_RETURN(MODULENAME, sysTickPtr);
        
    sysTickPtr->secCount = getuSecTicks() / TIME_uS_PER_SEC;
    sysTickPtr->hrCount = getHourTicks();
    
    if(sysTickPtr->secCount_Last != sysTickPtr->secCount)
    {
        sysTickPtr->secCount_Last = sysTickPtr->secCount;
        sysTickPtr->minCount = (sysTickPtr->secCount / TIME_SEC_PER_MIN);
        
        if(sysTickPtr->minCount != sysTickPtr->minCount_Last)
        {
            sysTickPtr->minCount_Last = sysTickPtr->minCount;
            /* Toggle red LED. 1 time per minute*/
            if(sysTickPtr->MinLEDvalue == sysTickPtr->Light_Off)
                sysTickPtr->MinLEDvalue = !sysTickPtr->Light_Off;
            else
                sysTickPtr->MinLEDvalue = sysTickPtr->Light_Off;
            
            WriteMinLED(sysTickPtr); 
        }        
        
        /* Toggle blue LED. 1 time per second */
        if(sysTickPtr->SecLEDvalue == sysTickPtr->Light_Off)
            sysTickPtr->SecLEDvalue = !sysTickPtr->Light_Off;
        else
            sysTickPtr->SecLEDvalue = sysTickPtr->Light_Off;
        WriteSecLED(sysTickPtr);
        
        /* Print system time to IO Device every second */
        SerializeTimeString(sysTickPtr);

        WriteTimeSerial(sysTickPtr);
    }
    return RETURN_SUCCESS;
}
 
MODULE_FUNC_PROTO_SYSTICK(MODULENAME) { ; }  // do nothing in the systick area

#endif //!EXAMPLE_SYSTICK

///////////////////////////////////////////////////////////////////////
// Attenuators UI Example
///////////////////////////////////////////////////////////////////////
#ifdef EXAMPLE_ATTEN_UI

// Re-usable, portable, cross-platform (systick example create() function)
MODSTRUCT_CREATE_PROTO(MODULENAME)
{
    MODSTRUCT(MODULENAME) outStruct;
    outStruct.compMod = CreateComputeModuleStruct();
    
    return outStruct;
}



// Re-usable, portable, cross-platform (systick example setup() function)
MODULE_FUNC_PROTO_SETUP(MODULENAME)
{
    MODDATAPTR_ERROR_RETURN(MODULENAME, AttenUIPtr);

    // Setup is running in the loop area to handle exceptions...
    IF_MODULE_ERROR(AttenUIPtr)
    {
        CLEAR_MODULE_ERRORS(AttenUIPtr);  // do nothing, clear flags
    }
    // Setup is running in the setup area following power on
    else
    {
        
    }
    return RETURN_SUCCESS;
}

// Re-usable, portable, cross-platform (systick example loop() function)
MODULE_FUNC_PROTO_LOOP(MODULENAME)
{
    MODDATAPTR_ERROR_RETURN(MODULENAME, AttenUIPtr);

    
    return RETURN_SUCCESS;
}

MODULE_FUNC_PROTO_SYSTICK(MODULENAME) { ; }  // do nothing in the systick area

#endif //!Attenuators UI Example
