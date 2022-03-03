/** \file APTmodule.h
*   \brief <a href="https://www.inmechasol.org/" target="_blank">IMS</a>:
		<a href="https://github.com/InMechaSol/ccNOos" target="_blank">ccNOos</a>,
		Declarations for straight C and C++ 

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

#ifndef __APTmODULEH__
#define __APTmODULEH__  

#include "NEO_LEA_M8T.h"
#include "HMR3300.h"



// APT Module Data Structure
struct gpsStruct
{
    struct SerialDeviceStruct* devptr;
    struct gpsData data;
    UI_8 newGPSData;
    UI_8 Connected;
};
struct gpsStruct creategpsStruct();
UI_8 readGPS(struct gpsStruct* gpsStructPtrIn);

struct eCompStruct
{
    struct SerialDeviceStruct* devptr;
    struct eCompDataStruct data;
    UI_8 neweCompassData;
    UI_8 Connected;
};
struct eCompStruct createeCompStruct();
UI_8 readEcompass(struct eCompStruct* eCompStructPtrIn);

struct aptStruct
{
    struct gpsStruct GPS;
    struct eCompStruct eCompass;    
};
struct aptStruct createaptStruct();
void tryReadAPTData(struct aptStruct* aptStructPtrIn);

// WMM Data Structure
struct wmmStruct
{
    float magdeclination;
    // save non-volatile...
    float lastGoodThreshold; // meters
    float lastGoodMultiplier; // deg per meter
    float lastGoodlatitude;
    float lastGoodlongitude;
    float lastGoodaltitude;
    float lastGoodmagdeclination;
};
struct wmmStruct createwmmStruct();



#ifdef __USINGCONSOLEMENU

void parseAPTMenuAPI(struct aptStruct* aptStructPtrIn, struct uiStruct* uiStructPtrIn);
void writeAPTMenuScreen(struct aptStruct* aptStructPtrIn, struct uiStruct* uiStructPtrIn);
void writeAPTLogLine(struct aptStruct* aptStructPtrIn, struct logStruct* logStructPtrIn);
void parseAPTConfigLine(struct aptStruct* aptStructPtrIn, struct configStruct* configStructPtrIn);

#endif

#ifdef __cplusplus


#endif // !__cplusplus
#endif // !__APTmODULEH__
