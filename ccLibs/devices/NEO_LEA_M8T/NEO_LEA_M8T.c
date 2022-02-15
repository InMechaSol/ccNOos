#include "NEO_LEA_M8T.h"

struct gpsData createGPSDataStruct()
{
    struct gpsData outStruct;
    outStruct.altitude = 0;
    outStruct.day = 0;
    outStruct.lattitude = 0;
    outStruct.longitude = 0;
    outStruct.month = 0;
    outStruct.utctime = 0;
    outStruct.year = 0;
    return outStruct;
}

// when possible, simply return snprintf() from std lib
// if using in ccNOos and/or ccOS framework, no work to be done
//  - these functions are defined by the platfrom specification
extern UI_8 ATO_F(const char* str, float* val);
extern UI_8 ATO_I16(const char* str, I_16* val);

UI_8 tryParseZDAString(char* gpsStringin, struct gpsData* gpsDataPtr, int iZDA, int commaCount)
{
    if (commaCount > 3)
    {
        commaCount = 0;
        int lastCommaIndex = 0;

        for (int i = iZDA; i < charBuffMax; i++)
        {
            if (gpsStringin[i] == ',')
            {
                commaCount++;
                if (i > lastCommaIndex + 1)
                {
                    // temporarily null terminate token sub string
                    gpsStringin[i] = '\0';

                    switch (commaCount)
                    {
                    case 2: // day
                        if (!ATO_I16(&gpsStringin[lastCommaIndex + 1], &gpsDataPtr->day))
                        {
                            gpsStringin[i] = ',';
                            return ui8FALSE;
                        }
                        break;
                    case 3: // month                        
                        if (!ATO_I16(&gpsStringin[lastCommaIndex + 1], &gpsDataPtr->month))
                        {
                            gpsStringin[i] = ',';
                            return ui8FALSE;
                        }
                        break;
                    case 4: // year
                        if (!ATO_I16(&gpsStringin[lastCommaIndex + 1], &gpsDataPtr->year))
                        {
                            gpsStringin[i] = ',';
                            return ui8FALSE;
                        }
                        break;
                    }
                    // reset to comma
                    gpsStringin[i] = ',';
                }
                lastCommaIndex = i;
            }
        }
        
    }
    return ui8TRUE;
}

UI_8 tryParseCGAString(char* gpsStringin, struct gpsData* gpsDataPtr, int iCGA, int commaCount)
{
    if (commaCount > 8)
    {
        commaCount = 0;
        int lastCommaIndex = 0;
        int I_dot = 0;
        float minutes = 0;


        for (int i = iCGA; i < charBuffMax; i++)
        {
            if (gpsStringin[i] == ',')
            {
                commaCount++;
                if (i > lastCommaIndex + 1)
                {
                    // temporarily null terminate token sub string
                    gpsStringin[i] = '\0';

                    switch (commaCount)
                    {
                    case 1: // utc time
                        if (!ATO_F(&gpsStringin[lastCommaIndex + 1], &gpsDataPtr->utctime))
                        {
                            gpsStringin[i] = ',';
                            return ui8FALSE;
                        }
                        break;
                    case 2: // latitude                              
                        I_dot = lastCommaIndex;
                        while (gpsStringin[++I_dot] != '.') { if (I_dot > charBuffMax - 2) break; }

                        if (I_dot < i)
                        {
                            gpsStringin[I_dot] = '\0';

                            if (ATO_F(&gpsStringin[lastCommaIndex + 1], &gpsDataPtr->lattitude))
                            {
                                if (ATO_F(&gpsStringin[I_dot + 1], &minutes))
                                {
                                    gpsDataPtr->lattitude += minutes / 60.0;
                                }
                                else
                                {
                                    gpsStringin[I_dot] = '.';
                                    gpsStringin[i] = ',';
                                    return ui8FALSE;
                                }
                            }
                            else
                            {
                                gpsStringin[I_dot] = '.';
                                gpsStringin[i] = ',';
                                return ui8FALSE;
                            }
                        }
                        else
                        {
                            gpsStringin[i] = ',';
                            return ui8FALSE;
                        }
                        break;
                    case 3: // latitude direction
                        if (gpsStringin[lastCommaIndex + 1] == 'S')
                            gpsDataPtr->lattitude = -gpsDataPtr->lattitude;
                        break;
                    case 4: // longitude
                        I_dot = lastCommaIndex;
                        while (gpsStringin[++I_dot] != '.') { if (I_dot > charBuffMax - 2) break; }

                        if (I_dot < i)
                        {
                            gpsStringin[I_dot] = '\0';

                            if (ATO_F(&gpsStringin[lastCommaIndex + 1], &gpsDataPtr->longitude))
                            {
                                if (ATO_F(&gpsStringin[I_dot + 1], &minutes))
                                {
                                    gpsDataPtr->longitude += minutes / 60.0;
                                    return ui8TRUE;
                                }
                                else
                                {
                                    gpsStringin[I_dot] = '.';
                                    gpsStringin[i] = ',';
                                    return ui8FALSE;
                                }
                            }
                            else
                            {
                                gpsStringin[I_dot] = '.';
                                gpsStringin[i] = ',';
                                return ui8FALSE;
                            }
                            gpsStringin[I_dot] = '.';
                        }
                        else
                        {
                            gpsStringin[i] = ',';
                            return ui8FALSE;
                        }
                        break;
                    case 5: // longitude direction
                        if (gpsStringin[lastCommaIndex + 1] == 'W')
                            gpsDataPtr->longitude = -gpsDataPtr->longitude;
                        break;
                    case 9: // altitude
                        if (!ATO_F(&gpsStringin[lastCommaIndex + 1], &gpsDataPtr->altitude))
                        {
                            gpsStringin[i] = ',';
                            return ui8FALSE;
                        }
                        break;
                    }
                    // reset to comma
                    gpsStringin[i] = ',';
                }
                lastCommaIndex = i;
            }
        }
    }
    return ui8TRUE;
}

UI_8 tryParseGPSData(char* gpsStringin, struct gpsData* gpsDataPtr)
{
    // check string begins with $
    if (gpsStringin[0] == '$')
    {
        int Istar = 0;
        int CommaCount = 0;
        int I_CGA = 0;
        int I_ZDA = 0;

        // scan message for valid tokens
        for (int i = 2; i < charBuffMax; i++)
        {
            // find ZDA and/or CGA token(s)
            if(gpsStringin[i]=='A')
            { 
                // ZDA
                if (gpsStringin[i - 1] == 'D')
                {
                    if (gpsStringin[i - 2] == 'Z')
                    {
                        I_ZDA = i - 2;
                    }
                }
                // CGA
                else if (gpsStringin[i - 1] == 'G')
                {
                    if (gpsStringin[i - 2] == 'C')
                    {
                        I_CGA = i - 2;
                    }
                }
            }

            // count all commas
            if (gpsStringin[i] == ',')
                CommaCount++;

            // find star
            if (gpsStringin[i] == '*')
            {
                Istar = i;
                break;
            }
        }

        // check message is valid
        if (Istar > (2+CommaCount))
        {
            // try parse CGA string
            if (I_CGA > 1)
            {
                return tryParseCGAString(gpsStringin, gpsDataPtr, I_CGA, CommaCount);
            }
            // try parse ZDA string
            if (I_ZDA > 1)
            {
                return tryParseZDAString(gpsStringin, gpsDataPtr, I_ZDA, CommaCount);
            }
        }
    }
    return ui8FALSE;
}

