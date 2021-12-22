///////////////////////////////////////////////////////////////////////
// SysTick Example
///////////////////////////////////////////////////////////////////////
#ifdef EXAMPLE_SYSTICK

#include "SysTickExample.h"

// Re-usable, portable, cross-platform (systick example create() function)
//struct SysTickClockStruct CreateSysTickClockStruct(int lightOff)
//MODdeclareSTRUCT(Mn) MODstructCREATE(Mn)(MODdeclareCREATEINS)
//MODdeclareCREATE(Mn)(MODdeclareCREATEINS);
MODdeclareCREATE(Mn)(MODdeclareCREATEINS)
{
    uint8_t i = 0;
    MODdeclareSTRUCT(Mn) outStruct;
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
MODdeclareSETUP(Mn)
{
    MODDATAPTR_ERROR_RETURN(Mn);

    // Setup is running in the loop area to handle exceptions...
    IF_MODULE_ERROR(Mn)
    {
        CLEAR_MODULE_ERRORS(Mn);  // do nothing, clear flags
    }
    // Setup is running in the setup area following power on
    else
    {
        // Values are initialized at instantiation
        // need to write to LEDs
        WriteMinLED(MODdataPTR(Mn));
        WriteSecLED(MODdataPTR(Mn));
    }
    return RETURN_SUCCESS;
}

// Re-usable, portable, cross-platform (systick example loop() function)
MODdeclareLOOP(Mn)
{
    MODDATAPTR_ERROR_RETURN(Mn);

    MODdataPTR(Mn)->secCount = getuSecTicks() / TIME_uS_PER_SEC;
    MODdataPTR(Mn)->hrCount = getHourTicks();

    if (MODdataPTR(Mn)->secCount_Last != MODdataPTR(Mn)->secCount)
    {
        MODdataPTR(Mn)->secCount_Last = MODdataPTR(Mn)->secCount;
        MODdataPTR(Mn)->minCount = (MODdataPTR(Mn)->secCount / TIME_SEC_PER_MIN);

        if (MODdataPTR(Mn)->minCount != MODdataPTR(Mn)->minCount_Last)
        {
            MODdataPTR(Mn)->minCount_Last = MODdataPTR(Mn)->minCount;
            /* Toggle red LED. 1 time per minute*/
            if (MODdataPTR(Mn)->MinLEDvalue == MODdataPTR(Mn)->Light_Off)
                MODdataPTR(Mn)->MinLEDvalue = !MODdataPTR(Mn)->Light_Off;
            else
                MODdataPTR(Mn)->MinLEDvalue = MODdataPTR(Mn)->Light_Off;

            WriteMinLED(MODdataPTR(Mn));
        }

        /* Toggle blue LED. 1 time per second */
        if (MODdataPTR(Mn)->SecLEDvalue == MODdataPTR(Mn)->Light_Off)
            MODdataPTR(Mn)->SecLEDvalue = !MODdataPTR(Mn)->Light_Off;
        else
            MODdataPTR(Mn)->SecLEDvalue = MODdataPTR(Mn)->Light_Off;
        WriteSecLED(MODdataPTR(Mn));

        /* Print system time to IO Device every second */
        SerializeTimeString(MODdataPTR(Mn));

        WriteTimeSerial(MODdataPTR(Mn));
    }
    return RETURN_SUCCESS;
}

MODdeclareSYSTICK(Mn) { ; }  // do nothing in the systick area

#endif //!EXAMPLE_SYSTICK
