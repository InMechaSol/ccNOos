///////////////////////////////////////////////////////////////////////
// SysTick Example
///////////////////////////////////////////////////////////////////////
#ifdef EXAMPLE_SYSTICK

#include "SysTickExample.h"

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
    for (i = 0; i < TIME_STR_LEN; i++)
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

    if (sysTickPtr->secCount_Last != sysTickPtr->secCount)
    {
        sysTickPtr->secCount_Last = sysTickPtr->secCount;
        sysTickPtr->minCount = (sysTickPtr->secCount / TIME_SEC_PER_MIN);

        if (sysTickPtr->minCount != sysTickPtr->minCount_Last)
        {
            sysTickPtr->minCount_Last = sysTickPtr->minCount;
            /* Toggle red LED. 1 time per minute*/
            if (sysTickPtr->MinLEDvalue == sysTickPtr->Light_Off)
                sysTickPtr->MinLEDvalue = !sysTickPtr->Light_Off;
            else
                sysTickPtr->MinLEDvalue = sysTickPtr->Light_Off;

            WriteMinLED(sysTickPtr);
        }

        /* Toggle blue LED. 1 time per second */
        if (sysTickPtr->SecLEDvalue == sysTickPtr->Light_Off)
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