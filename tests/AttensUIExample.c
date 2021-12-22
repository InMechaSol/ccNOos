///////////////////////////////////////////////////////////////////////
// Attenuators UI Example
///////////////////////////////////////////////////////////////////////
#ifndef COMPILE_TESTS
#ifdef EXAMPLE_ATTEN_UI

#include "AttensUIExample.h"

// Re-usable, portable, cross-platform (systick example create() function)
MODdeclareCREATE(Mn)(MODdeclareCREATEINS)
{
    int i;
    MODdeclareSTRUCT(Mn) outStruct;
    outStruct.compMod = CreateComputeModuleStruct();
    outStruct.API_DATValue = 0;
    for (i = 0; i < MAX_NUM_ATTENUATORS; i++)
    {
        outStruct.AttenuatorNeedsWriting[i] = true;
        outStruct.AttenuatorValues[i] = MAX_ATTEN_VAL;
    }
    outStruct.chars2Write = 0;
    outStruct.CMD_AttenuatorBits = 0;
    for (i = 0; i < CONSOLE_LINE_LEN; i++)
    {
        outStruct.consoleLine[i] = 0x00;
        outStruct.apiLine[i] = 0x00;
    }
    outStruct.INDEX_Attenuator = 0;
    outStruct.charsRead = 1;
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

    }
    return RETURN_SUCCESS;
}

void limitDATcmd(float* DATptr)
{
    float fracPart, intPart;
    if (*DATptr < MIN_ATTEN_VAL)
        *DATptr = MIN_ATTEN_VAL;
    else if (*DATptr > MAX_ATTEN_VAL)
        *DATptr = MAX_ATTEN_VAL;

    fracPart = ModuloFloat(*DATptr, &intPart);

    if (fracPart > 0.75)
        fracPart = 0.75;
    else if (fracPart < 0.25)
        fracPart = 0.0;
    else if (fracPart > 0.25 && fracPart < 0.50)
        fracPart = 0.25;
    else if (fracPart < 0.75 && fracPart > 0.50)
        fracPart = 0.50;

    *DATptr = intPart + fracPart;
}

void CalcAttenuationBits(MODdeclarePTRIN(Mn))
{
    // cmd is limited in the static handler for MCU/DAT

    uint8_t intPart = 0b00011111 & ((uint8_t)MODdataPTR(Mn)->AttenuatorValues[MODdataPTR(Mn)->INDEX_Attenuator]);
    float fracPart = MODdataPTR(Mn)->AttenuatorValues[MODdataPTR(Mn)->INDEX_Attenuator] - intPart;

    bool bit16 = (0b00010000 & intPart) >> 4;
    bool bit8 = (0b00001000 & intPart) >> 3;
    bool bit4 = (0b00000100 & intPart) >> 2;
    bool bit2 = (0b00000010 & intPart) >> 1;
    bool bit1 = (0b00000001 & intPart);
    bool bit0_50 = (fracPart > 0.49);
    bool bit0_25 = (fracPart > 0.74);
    if (fracPart < 0.49 && fracPart > 0.0)
        bit0_25 = 1;



    MODdataPTR(Mn)->CMD_AttenuatorBits = (bit16 & 0x01) << 7;
    MODdataPTR(Mn)->CMD_AttenuatorBits |= (bit8 & 0x01) << 6;
    MODdataPTR(Mn)->CMD_AttenuatorBits |= (bit4 & 0x01) << 5;
    MODdataPTR(Mn)->CMD_AttenuatorBits |= (bit2 & 0x01) << 4;
    MODdataPTR(Mn)->CMD_AttenuatorBits |= (bit1 & 0x01) << 3;
    MODdataPTR(Mn)->CMD_AttenuatorBits |= (bit0_50 & 0x01) << 2;
    MODdataPTR(Mn)->CMD_AttenuatorBits |= (bit0_25 & 0x01) << 1;

}

void PrintMenuText(MODdeclarePTRIN(Mn))
{
    INIT_MENU_VARS(CONSOLE_LINE_LEN, MODdataPTR(Mn)->consoleLine);
    // looping and printing
    // for looping...
    int lines2Print = 1;
    int linesPrinted = 0;
    if (MODdataPTR(Mn)->charsRead > 0)
    {
        MODdataPTR(Mn)->charsRead = 0;
        while (lines2Print > 0)
        {
            switch (linesPrinted)
            {
            case 0:
                PRINT_MENU_LN  "\033[2J\033[0;0H\n////////// Digital Attenuator UI"     END_MENU_LN;
            case 1:
                PRINT_MENU_LN  "\nCurrent Attenuation Tx:\t%6.2f dB", MODdataPTR(Mn)->AttenuatorValues[0]      END_MENU_LN;
            case 2:
                PRINT_MENU_LN  "\nCurrent Attenuation Rx:\t%6.2f dB", MODdataPTR(Mn)->AttenuatorValues[1]      END_MENU_LN;
            case 3:
                PRINT_MENU_LN  "\nCurrent Attenuation Xx:\t%6.2f dB", MODdataPTR(Mn)->AttenuatorValues[2]      END_MENU_LN;
            case 4:
                PRINT_MENU_LN  "\n//////////////////////////////////"      END_MENU_LN;
            case 5:
                PRINT_MENU_LN  "\nType \"Atten:Tx:22.75;\" set Tx to 22.75"      END_MENU_LN;
            case 6:
                PRINT_MENU_LN  "\nType \"Atten:Rx:21.00;\" set Rx to 21.00"      END_MENU_LN;
            case 7:
                PRINT_MENU_LN  "\nType \"Atten:Xx:19.25;\" set Xx to 19.25"      END_MENU_LN;
            case 8:
                PRINT_MENU_LN "\nInput>>" END_MENU_LN;
            default:
                lines2Print = 0;
                break;
            }

            MODdataPTR(Mn)->chars2Write = charsWritten;
            linesPrinted++;

            if (lines2Print > 0)
                WriteMenuLine(&MODdataPTR(Mn)->consoleLine[0]);
        }
    }

}

void ParseAPIString(MODdeclarePTRIN(Mn))
{

    // loop characters from user input
    int i = 0;
    int j = 0, k = 0, l = 0;

#define thisC MODdataPTR(Mn)->apiLine[i]

    while ((thisC != 0x00) && (i < CONSOLE_LINE_LEN))
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
                return;

            // j index of first :
            // k index of second :
            // i index of terminator ;

            MODdataPTR(Mn)->apiLine[j] = 0x00;
            if (stringMatchCaseSensitive(&MODdataPTR(Mn)->apiLine[0], "Atten"))
            {
                MODdataPTR(Mn)->apiLine[k] = 0x00;
                if (stringMatchCaseSensitive(&MODdataPTR(Mn)->apiLine[j + 1], "Tx"))
                {
                    l = 0;
                }
                else if (stringMatchCaseSensitive(&MODdataPTR(Mn)->apiLine[j + 1], "Rx"))
                {
                    l = 1;
                }
                else if (stringMatchCaseSensitive(&MODdataPTR(Mn)->apiLine[j + 1], "Xx"))
                {
                    l = 2;
                }
                else
                    return;

                thisC = 0x00;
                if (ATO_F(&MODdataPTR(Mn)->apiLine[k + 1], &MODdataPTR(Mn)->AttenuatorValues[l]))
                {
                    // limit command within range
                    limitDATcmd(&MODdataPTR(Mn)->AttenuatorValues[l]);
                    // set flag to update
                    MODdataPTR(Mn)->AttenuatorNeedsWriting[l] = true;
                }

            }
            else
                return;
        }
        i++;
    }
#undef thisC
}

// Re-usable, portable, cross-platform (systick example loop() function)
MODdeclareLOOP(Mn)
{
    MODDATAPTR_ERROR_RETURN(Mn);

    int i;
    for (i = 0; i < MAX_NUM_ATTENUATORS; i++)
    {
        if (MODdataPTR(Mn)->AttenuatorNeedsWriting[i])
        {
            MODdataPTR(Mn)->INDEX_Attenuator = i;
            CalcAttenuationBits(MODdataPTR(Mn));

            WriteAttenuators(MODdataPTR(Mn));

            MODdataPTR(Mn)->AttenuatorNeedsWriting[i] = false;
        }
    }
    PrintMenuText(MODdataPTR(Mn));
    // to block or not to block?
    ReadUserInput(MODdataPTR(Mn));
    // parse api string from console input
    ParseAPIString(MODdataPTR(Mn));
    return RETURN_SUCCESS;
}

MODdeclareSYSTICK(Mn) { ; }  // do nothing in the systick area

#endif //!Attenuators UI Example
#endif
