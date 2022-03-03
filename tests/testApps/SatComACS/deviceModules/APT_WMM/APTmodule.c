/** \file APTmodule.c
*   \brief part of <a href="https://github.com/InMechaSol/ccNOos">ccNOos</a>, Implementation for straight C 

    Copyright 2021 InMechaSol, Inc

    https://www.inmechasol.org/

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

#include "APTmodule.h"



struct gpsStruct creategpsStruct()
{
    struct gpsStruct outStruct;
    outStruct.devptr = nullptr;
    outStruct.data = createGPSDataStruct();
    outStruct.Connected = ui8FALSE;
    outStruct.newGPSData = ui8FALSE;
    return outStruct;
}
struct eCompStruct createeCompStruct()
{
    struct eCompStruct outStruct;
    outStruct.devptr = nullptr;
    outStruct.Connected = ui8FALSE;
    outStruct.neweCompassData = ui8FALSE;
    outStruct.data = createEcompDataStruct();
    return outStruct;
}
struct aptStruct createaptStruct()
{
    struct aptStruct outStruct;
    outStruct.eCompass = createeCompStruct();
    outStruct.GPS = creategpsStruct();
    return outStruct;
}
struct wmmStruct createwmmStruct()
{
    struct wmmStruct outStruct;
    outStruct.magdeclination = 0;
    outStruct.lastGoodThreshold = 0;
    outStruct.lastGoodMultiplier = 0;
    outStruct.lastGoodmagdeclination = 0;
    outStruct.lastGoodlongitude = 0;
    outStruct.lastGoodlatitude = 0;
    outStruct.lastGoodaltitude = 0;
    return outStruct;
}

void tryReadAPTData(struct aptStruct* aptStructPtrIn)
{
    // try read gps, if got string(s)
    if (readGPS(&aptStructPtrIn->GPS))
    {
        // try parse strings
        if (tryParseGPSData(&aptStructPtrIn->GPS.devptr->devdata.inbuff.charbuff[0], &aptStructPtrIn->GPS.data))
        {
            aptStructPtrIn->GPS.newGPSData = ui8TRUE; // this signals to state machine that new gps data is available from gps device
        }
        aptStructPtrIn->GPS.devptr->devdata.newDataReadIn = ui8FALSE;
    }
    // try read eCompass, if got string(s)
    if (readEcompass(&aptStructPtrIn->eCompass))
    {
        // try parse strings
        if (tryParseEcompData(&aptStructPtrIn->eCompass.devptr->devdata.inbuff.charbuff[0], &aptStructPtrIn->eCompass.data))
        {
            aptStructPtrIn->eCompass.neweCompassData = ui8TRUE; // this signals to state machine that new gps data is available from gps device
        }
        aptStructPtrIn->eCompass.devptr->devdata.newDataReadIn = ui8FALSE;
    }
}

