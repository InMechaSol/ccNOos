/** \file NEO_LEA_M8T.c
*   \brief <a href="https://www.inmechasol.org/" target="_blank">IMS</a>:
		<a href="https://github.com/InMechaSol/ccNOos" target="_blank">ccNOos</a>,
		Implementation for straight C 

    Copyright 2021 <a href="https://www.inmechasol.org/" target="_blank">InMechaSol, Inc</a>    

    Licensed under the Apache License, Version 2.0(the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

Notes:
	(.c includes .h) - for straight C or
	(.cpp includes .c which includes .h) - for C++ wrapped straight C
	*Always compiled to a single compilation unit, either C or CPP, not both

*/

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


UI_8 tryParseZDAString(char* gpsStringin, struct gpsData* gpsDataPtr, int iZDA, int commaCount)
{
    if (commaCount > 4)
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
                    case 3: // day
                        if (!ATO_I16(&gpsStringin[lastCommaIndex + 1], &gpsDataPtr->day))
                        {
                            gpsStringin[i] = ',';
                            return ui8FALSE;
                        }
                        break;
                    case 4: // month
                        if (!ATO_I16(&gpsStringin[lastCommaIndex + 1], &gpsDataPtr->month))
                        {
                            gpsStringin[i] = ',';
                            return ui8FALSE;
                        }
                        break;
                    case 5: // year
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

UI_8 tryParseGGAString(char* gpsStringin, struct gpsData* gpsDataPtr, int iGGA, int commaCount)
{
    if (commaCount > 9)
    {
        commaCount = 0;
        int lastCommaIndex = 0;
        int I_dot = 0;
        float minutes = 0;
        char tempChar = 0x00;


        for (int i = iGGA; i < charBuffMax; i++)
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
                    case 2: // utc time
                        if (!ATO_F(&gpsStringin[lastCommaIndex + 1], &gpsDataPtr->utctime))
                        {
                            gpsStringin[i] = ',';
                            return ui8FALSE;
                        }
                        break;
                    case 3: // latitude
                        I_dot = lastCommaIndex;
                        while (gpsStringin[++I_dot] != '.') { if (I_dot > charBuffMax - 2) break; }

                        if (I_dot < i)
                        {
                            tempChar = gpsStringin[I_dot-2];
                            gpsStringin[I_dot-2] = '\0';

                            if (ATO_F(&gpsStringin[lastCommaIndex + 1], &gpsDataPtr->lattitude))
                            {
                                gpsStringin[I_dot-2] = tempChar;
                                if (ATO_F(&gpsStringin[I_dot-2], &minutes))
                                {
                                    gpsDataPtr->lattitude += minutes / 60.0f;
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
                    case 4: // latitude direction
                        if (gpsStringin[lastCommaIndex + 1] == 'S')
                            gpsDataPtr->lattitude = -gpsDataPtr->lattitude;
                        break;
                    case 5: // longitude
                        I_dot = lastCommaIndex;
                        while (gpsStringin[++I_dot] != '.') { if (I_dot > charBuffMax - 2) break; }



                        if (I_dot < i)
                        {
                            tempChar = gpsStringin[I_dot-2];
                            gpsStringin[I_dot-2] = '\0';

                            if (ATO_F(&gpsStringin[lastCommaIndex + 1], &gpsDataPtr->longitude))
                            {
                                gpsStringin[I_dot-2] = tempChar;

                                if (ATO_F(&gpsStringin[I_dot-2], &minutes))
                                {
                                    gpsDataPtr->longitude += minutes / 60.0f;
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
                    case 6: // longitude direction
                        if (gpsStringin[lastCommaIndex + 1] == 'W')
                            gpsDataPtr->longitude = -gpsDataPtr->longitude;
                        break;
                    case 10: // altitude
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
//    if (gpsStringin[0] == '$')
//    {
        int CommaCount = 0;
        int I_GGA = 0;
        int I_ZDA = 0;
        int I_GNS = 0;
        UI_8 retVal = ui8FALSE;

        // scan message for valid tokens
        for (int i = 5; i < charBuffMax; i++)
        {
            // find ZDA or GGA token
            if(gpsStringin[i]=='A')
            { 
                // ZDA
                if (gpsStringin[i - 1] == 'D')
                {
                    if (gpsStringin[i - 2] == 'Z' && gpsStringin[i - 5] == '$')
                    {
                        I_ZDA = i - 2;
                    }
                }
                // GGA
                else if (gpsStringin[i - 1] == 'G')
                {
                    if (gpsStringin[i - 2] == 'G' && gpsStringin[i - 5] == '$')
                    {
                        I_GGA = i - 2;
                    }
                }
            }
            else if(gpsStringin[i]=='S')
            {
                // GNS
                if (gpsStringin[i - 1] == 'N')
                {
                    if (gpsStringin[i - 2] == 'G' && gpsStringin[i - 5] == '$')
                    {
                        I_GNS = i - 2;
                    }
                }
            }

            // count all commas
            if (gpsStringin[i] == ',')
                CommaCount++;

            // find star
            if (gpsStringin[i] == '*')
            {                
                // try parse GGA string
                if (I_GGA > 0)
                {
                    if(tryParseGGAString(gpsStringin, gpsDataPtr, I_GGA, CommaCount)==ui8TRUE)
                        retVal |= ui8TRUE ;
                    I_GGA = 0;
                }
                // try parse ZDA string
                else if (I_ZDA > 0)
                {
                    if(tryParseZDAString(gpsStringin, gpsDataPtr, I_ZDA, CommaCount)==ui8TRUE)
                        retVal |= ui8TRUE;
                    I_ZDA = 0;

                }
                // try parse GNS
                else if (I_GNS > 0)
                {
                    if(tryParseGGAString(gpsStringin, gpsDataPtr, I_GNS, CommaCount)==ui8TRUE)
                        retVal |= ui8TRUE;
                    I_GNS = 0;
                }
                CommaCount = 0;

            }
        }


//    }
    return retVal;
}

