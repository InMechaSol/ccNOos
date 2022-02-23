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
    int commaIndex0 = 0;
    int commaIndex1 = 0;

    // scan message for valid tokens
    for (int i = 2; i < charBuffMax; i++)
    {
        // find all commas
        if (eCompStringin[i] == ',')
        {
            if(commaIndex0==0)
                commaIndex0 = i;
            else if(commaIndex1==0)
                commaIndex1 = i;
            else
                return ui8FALSE;
        }

        // find carriage return
        if (eCompStringin[i] == '\r')
        {
            if((commaIndex0>1) && (commaIndex1>(commaIndex0+1)) && (i > (commaIndex1+1)))
            {
                // yaw
                eCompStringin[commaIndex0] = 0x00;
                if (!ATO_F(&eCompStringin[0], &eCompDataPtr->yaw))
                {
                    eCompStringin[commaIndex0] = ',';
                    return ui8FALSE;
                }
                // pitch
                eCompStringin[commaIndex1] = 0x00;
                if (!ATO_F(&eCompStringin[commaIndex0 + 1], &eCompDataPtr->pitch))
                {
                    eCompStringin[commaIndex1] = ',';
                    return ui8FALSE;
                }
                // roll
                eCompStringin[i] = 0x00;
                if (!ATO_F(&eCompStringin[commaIndex1 + 1], &eCompDataPtr->roll))
                {
                    eCompStringin[i] = '\r';
                    return ui8FALSE;
                }
                eCompStringin[commaIndex0] = ',';
                eCompStringin[commaIndex1] = ',';
                eCompStringin[i] = '\r';
                return ui8TRUE;
            }
        }
    }
    return ui8FALSE;


}
