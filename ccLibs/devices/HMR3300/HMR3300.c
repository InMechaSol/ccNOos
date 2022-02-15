#include "HMR3300.h"

struct eCompDataStruct createEcompDataStruct()
{
	struct eCompDataStruct outStruct;
	outStruct.pitch = 0;
	outStruct.roll = 0;
	outStruct.yaw = 0;
	return outStruct;
}

// when possible, simply return snprintf() from std lib
// if using in ccNOos and/or ccOS framework, no work to be done
//  - these functions are defined by the platfrom specification
extern UI_8 ATO_F(const char* str, float* val);

UI_8 tryParseEcompData(char* eCompStringin, struct eCompDataStruct* eCompDataPtr)
{
    int Istar = -1;
    int I_S = -1;
    int CommaCount = 0;

    // scan message for valid tokens
    for (int i = 2; i < charBuffMax; i++)
    {
        // count all commas
        if (eCompStringin[i] == ',')
            CommaCount++;

        // find star
        if (eCompStringin[i] == '*')
        {
            Istar = i;
        }
        // find S
        if (eCompStringin[i] == '$')
        {
            I_S = i;
        }
    }

    // check message is valid
    if (Istar == -1 && I_S == -1)
    {
        if (CommaCount > 3)
        {
            CommaCount = 0;
            int lastCommaIndex = 0;

            for (int i = 0; i < charBuffMax; i++)
            {
                if (eCompStringin[i] == ',')
                {
                    CommaCount++;
                    if (i > lastCommaIndex + 1)
                    {
                        // temporarily null terminate token sub string
                        eCompStringin[i] = '\0';

                        switch (CommaCount)
                        {
                        case 2: // yaw
                            if (!ATO_F(&eCompStringin[lastCommaIndex + 1], &eCompDataPtr->yaw))
                            {
                                eCompStringin[i] = ',';
                                return ui8FALSE;
                            }
                            break;
                        case 3: // pitch                        
                            if (!ATO_F(&eCompStringin[lastCommaIndex + 1], &eCompDataPtr->pitch))
                            {
                                eCompStringin[i] = ',';
                                return ui8FALSE;
                            }
                            break;
                        case 4: // roll
                            if (!ATO_F(&eCompStringin[lastCommaIndex + 1], &eCompDataPtr->roll))
                            {
                                eCompStringin[i] = ',';
                                return ui8FALSE;
                            }
                            break;
                        }
                        // reset to comma
                        eCompStringin[i] = ',';
                    }
                    lastCommaIndex = i;
                }
            }
            return ui8TRUE;

        }
        else
            return ui8FALSE;
    }
    else
        return ui8FALSE;
}
