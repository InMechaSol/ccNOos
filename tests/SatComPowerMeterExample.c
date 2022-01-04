///////////////////////////////////////////////////////////////////////
// SatCom Tunable Power Meter Example
///////////////////////////////////////////////////////////////////////
#ifndef COMPILE_TESTS
#ifdef EXAMPLE_POWER_METER

#include "SatComPowerMeterExample.h"

// Re-usable, portable, cross-platform (systick example create() function)
MODdeclareCREATE(Mn)(MODdeclareCREATEINS)
{
    UI_8 i = 0;
    MODdeclareSTRUCT(Mn) outStruct;
    outStruct.compMod = CreateComputeModuleStruct();

    outStruct.ADC_ChipSelect = ui8FALSE;
    outStruct.DownConverter_ChipSelect = ui8FALSE;
    outStruct.Bytes2Write = 0;
    outStruct.BytesRead = 0;
    for (i = 0; i < SPIBYTESMAX; i++)
    {
        outStruct.SPIBytesIn[i] = 0;
        outStruct.SPIBytesOut[i] = 0;
    }
    outStruct.DownLinkIF_MHZ = 1457;

#ifdef __USINGCONSOLEMENU
    outStruct.printMReturn = 0;
    outStruct.parseIReturn = 0;
    outStruct.chars2Write = 0;
    outStruct.charsRead = 0;
    for (i = 0; i < charBuffMax; i++)
    {
        outStruct.charbuff_In[i] = 0;
        outStruct.charbuff_Out[i] = 0;
    }
#endif

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
        // TODO MARK: any setup code should go here
#ifdef __USINGCONSOLEMENU  
        MODdataPTR(Mn)->charbuff_In[0] = ';';  // this will trigger an initial print
#endif
    }
    return RETURN_SUCCESS;
}



// Re-usable, portable, cross-platform (systick example loop() function)
MODdeclareLOOP(Mn)
{
    MODDATAPTR_ERROR_RETURN(Mn);


    // TODO MARK: any loop code should go here
    if (MODdataPTR(Mn)->TriggerUpdate)
    {
        // code to update frequency...

        MODdataPTR(Mn)->TriggerUpdate = ui8FALSE;
    }

    // FYI, by default, this is blocking call to read stdin
#ifdef __USINGCONSOLEMENU  
    MODdataPTR(Mn)->printMReturn = MODprintMENU(Mn)(compModPtrIn);
    MODdataPTR(Mn)->parseIReturn = MODparseINPUT(Mn)(compModPtrIn);
#endif
    
    return RETURN_SUCCESS;
}

MODdeclareSYSTICK(Mn) { ; }  // do nothing in the systick area

#ifdef __USINGCONSOLEMENU            
MODdeclarePRINTm(Mn)
{
    MODDATAPTR_ERROR_RETURN(Mn);

    INIT_MENU_VARS(charBuffMax, MODdataPTR(Mn)->charbuff_Out);
    // looping and printing
    // for looping...
    int lines2Print = 1;
    int linesPrinted = 0;
    if (MODdataPTR(Mn)->charbuff_In[0] != 0x00 > 0)
    {
        MODdataPTR(Mn)->charsRead = 0;
        while (lines2Print > 0)
        {
            switch (linesPrinted)
            {
            case 0:
                PRINT_MENU_LN  "\033[2J\033[0;0H\n// Version %s %s //", ccNOosccNOos_VerString(), ccNOosccNOos_VerDateString()     END_MENU_LN;
                //PRINT_MENU_LN  "\n///////// Console Menu - ccNOos Tests /////////"     END_MENU_LN;
            case 1:
                PRINT_MENU_LN  "\n/////////// Power Meter Console Menu //////////"     END_MENU_LN;
            case 2:
                PRINT_MENU_LN  "\nDownlink IF Freq (MHz):\t%d", MODdataPTR(Mn)->DownLinkIF_MHZ      END_MENU_LN;
            case 3:
                PRINT_MENU_LN  "\nSPI Bytes Read:\t\t%d", MODdataPTR(Mn)->BytesRead      END_MENU_LN;
            case 4:
                PRINT_MENU_LN  "\nSPI Bytes 2 Write:\t%d", MODdataPTR(Mn)->Bytes2Write      END_MENU_LN;
            case 5:
                PRINT_MENU_LN  "\n///////////////////////////////////////////////"      END_MENU_LN;
            case 6:
                PRINT_MENU_LN  "\nType \"powerMeter:Freq:1400;\" set DownLink IF Freq (MHz) to 1400"      END_MENU_LN;
            case 7:
                PRINT_MENU_LN "\nInput>>" END_MENU_LN;
            default:
                lines2Print = 0;
                break;
            }

            MODdataPTR(Mn)->chars2Write = charsWritten;
            linesPrinted++;

            if (lines2Print > 0)
                WriteMenuLine(&MODdataPTR(Mn)->charbuff_Out[0]);
        }
    }
    return charsWritten;
}

MODdeclarePARSEi(Mn)
{
    MODDATAPTR_ERROR_RETURN(Mn);

    // loop characters from user input
    int i = 0;
    int j = 0, k = 0, l = 0;

    GetMenuChars(&MODdataPTR(Mn)->charbuff_In[0]);
    if (MODdataPTR(Mn)->charbuff_In[0] != 0x00)
    {
        MODdataPTR(Mn)->charsRead++;
#define thisC MODdataPTR(Mn)->charbuff_In[i]

        while ((thisC != 0x00) && (i < charBuffMax))
        {
            // find delimeter or terminator
            if (thisC == ASCII_colon)
            {
                if (j == 0)
                    j = i;
                else
                    k = i;
            }
            else if (thisC == ASCII_semicolon)
            {
                if (j == 0 || k == 0 || j >= k)
                    return 0u;

                // j index of first :
                // k index of second :
                // i index of terminator ;

                MODdataPTR(Mn)->charbuff_In[j] = 0x00;
                if (stringMatchCaseSensitive(&MODdataPTR(Mn)->charbuff_In[0], "powerMeter"))
                {
                    MODdataPTR(Mn)->charbuff_In[k] = 0x00;
                    if (stringMatchCaseSensitive(&MODdataPTR(Mn)->charbuff_In[j + 1], "Freq"))
                    {
                        MODdataPTR(Mn)->charbuff_In[i] = 0x00;
                        if (ATO_U32(&MODdataPTR(Mn)->charbuff_In[k + 1], &MODdataPTR(Mn)->DownLinkIF_MHZ))
                        {
                            MODdataPTR(Mn)->TriggerUpdate = ui8TRUE;
                        }
                    }
                    else
                        return 0u;

                    thisC = 0x00;

                }
                else
                    return 0u;
            }
            i++;
        }
    }
#undef thisC
    return i;
}
#endif

#endif //!EXAMPLE_POWER_METER
#endif