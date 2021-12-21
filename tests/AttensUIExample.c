///////////////////////////////////////////////////////////////////////
// Attenuators UI Example
///////////////////////////////////////////////////////////////////////
#ifndef COMPILE_TESTS
#ifdef EXAMPLE_ATTEN_UI

#include "AttensUIExample.h"

// Re-usable, portable, cross-platform (systick example create() function)
MODSTRUCT_CREATE_PROTO(MODULENAME)
{
    int i;
    MODSTRUCT(MODULENAME) outStruct;
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


void CalcAttenuationBits(MODSTRUCTPTR_IN(MODULENAME))
{
    // cmd is limited in the static handler for MCU/DAT

    uint8_t intPart = 0b00011111 & ((uint8_t)AttenUIDataPtrIn->AttenuatorValues[AttenUIDataPtrIn->INDEX_Attenuator]);
    float fracPart = AttenUIDataPtrIn->AttenuatorValues[AttenUIDataPtrIn->INDEX_Attenuator] - intPart;

    bool bit16 = (0b00010000 & intPart) >> 4;
    bool bit8 = (0b00001000 & intPart) >> 3;
    bool bit4 = (0b00000100 & intPart) >> 2;
    bool bit2 = (0b00000010 & intPart) >> 1;
    bool bit1 = (0b00000001 & intPart);
    bool bit0_50 = (fracPart > 0.49);
    bool bit0_25 = (fracPart > 0.74);
    if (fracPart < 0.49 && fracPart > 0.0)
        bit0_25 = 1;



    AttenUIDataPtrIn->CMD_AttenuatorBits = (bit16 & 0x01) << 7;
    AttenUIDataPtrIn->CMD_AttenuatorBits |= (bit8 & 0x01) << 6;
    AttenUIDataPtrIn->CMD_AttenuatorBits |= (bit4 & 0x01) << 5;
    AttenUIDataPtrIn->CMD_AttenuatorBits |= (bit2 & 0x01) << 4;
    AttenUIDataPtrIn->CMD_AttenuatorBits |= (bit1 & 0x01) << 3;
    AttenUIDataPtrIn->CMD_AttenuatorBits |= (bit0_50 & 0x01) << 2;
    AttenUIDataPtrIn->CMD_AttenuatorBits |= (bit0_25 & 0x01) << 1;

}



void PrintMenuText(MODSTRUCTPTR_IN(MODULENAME))
{
    INIT_MENU_VARS(CONSOLE_LINE_LEN, AttenUIDataPtrIn->consoleLine);
    // looping and printing
    // for looping...
    int lines2Print = 1;
    int linesPrinted = 0;
    if (AttenUIDataPtrIn->charsRead > 0)
    {
        AttenUIDataPtrIn->charsRead = 0;
        while (lines2Print > 0)
        {
            switch (linesPrinted)
            {
            case 0:
                PRINT_MENU_LN  "\033[2J\033[0;0H\n////////// Digital Attenuator UI"     END_MENU_LN;
            case 1:
                PRINT_MENU_LN  "\nCurrent Attenuation Tx:\t%6.2f dB", AttenUIDataPtrIn->AttenuatorValues[0]      END_MENU_LN;
            case 2:
                PRINT_MENU_LN  "\nCurrent Attenuation Rx:\t%6.2f dB", AttenUIDataPtrIn->AttenuatorValues[1]      END_MENU_LN;
            case 3:
                PRINT_MENU_LN  "\nCurrent Attenuation Xx:\t%6.2f dB", AttenUIDataPtrIn->AttenuatorValues[2]      END_MENU_LN;
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

            AttenUIDataPtrIn->chars2Write = charsWritten;
            linesPrinted++;

            if (lines2Print > 0)
                WriteMenuLine(&AttenUIDataPtrIn->consoleLine[0]);
        }
    }

}

void ParseAPIString(MODSTRUCTPTR_IN(MODULENAME))
{

    // loop characters from user input
    int i = 0;
    int j = 0, k = 0, l = 0;
#define thisC AttenUIDataPtrIn->apiLine[i]
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

            AttenUIDataPtrIn->apiLine[j] = 0x00;
            if (stringMatchCaseSensitive(&AttenUIDataPtrIn->apiLine[0], "Atten"))
            {
                AttenUIDataPtrIn->apiLine[k] = 0x00;
                if (stringMatchCaseSensitive(&AttenUIDataPtrIn->apiLine[j + 1], "Tx"))
                {
                    l = 0;
                }
                else if (stringMatchCaseSensitive(&AttenUIDataPtrIn->apiLine[j + 1], "Rx"))
                {
                    l = 1;
                }
                else if (stringMatchCaseSensitive(&AttenUIDataPtrIn->apiLine[j + 1], "Xx"))
                {
                    l = 2;
                }
                else
                    return;

                thisC = 0x00;
                if (ATO_F(&AttenUIDataPtrIn->apiLine[k + 1], &AttenUIDataPtrIn->AttenuatorValues[l]))
                {
                    // limit command within range
                    limitDATcmd(&AttenUIDataPtrIn->AttenuatorValues[l]);
                    // set flag to update
                    AttenUIDataPtrIn->AttenuatorNeedsWriting[l] = true;
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
MODULE_FUNC_PROTO_LOOP(MODULENAME)
{
    MODDATAPTR_ERROR_RETURN(MODULENAME, AttenUIPtr);

    int i;
    for (i = 0; i < MAX_NUM_ATTENUATORS; i++)
    {
        if (AttenUIPtr->AttenuatorNeedsWriting[i])
        {
            AttenUIPtr->INDEX_Attenuator = i;
            CalcAttenuationBits(AttenUIPtr);

            WriteAttenuators(AttenUIPtr);

            AttenUIPtr->AttenuatorNeedsWriting[i] = false;
        }
    }
    PrintMenuText(AttenUIPtr);
    // to block or not to block?
    ReadUserInput(AttenUIPtr);
    // parse api string from console input
    ParseAPIString(AttenUIPtr);
    return RETURN_SUCCESS;
}

MODULE_FUNC_PROTO_SYSTICK(MODULENAME) { ; }  // do nothing in the systick area

#endif //!Attenuators UI Example
#endif