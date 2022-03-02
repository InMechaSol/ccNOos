/** \file satComControl.h
*   \brief part of ccNOos, Declarations for straight C and C++ wrappers 

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

#ifndef __SATCOMCONTROL
#define __SATCOMCONTROL


#include "motionControl.h"


/////////////////////////////////////////////////////////////////////////////////////
// Data Structures
struct satelliteStruct
{
	float UpLinkFreq_kHz;
	float DownLinkFreq_kHz;
	float SSP_Lattitude;
	float SSP_Longitude;
	float SSP_Altitude;	
};
struct satelliteStruct createSatelliteStruct();


struct commsParameters
{
	float Rx_LO_MHz;
	float Tx_LO_MHz;
	float RxSignalStrengthMetric;
};
struct commsParameters createcommsParameters();


struct geoLocationStruct
{
	float latitude;
	float longitude;
	float altitude;
};
struct geoLocationStruct creategeoLocationStruct();


struct antennaAttitudeStruct
{
	float roll;
	float pitch;
	float yaw;
};
struct antennaAttitudeStruct createantennaAttitudeStruct();


struct antennaAxis
{
	struct axisStruct World;
	struct axisStruct Pedestal;
};
struct antennaAxis createantennaAxis();



enum antennaState
{
	antState_init,
	antState_NotAcquired,
	antState_Acquiring,
	antState_Acquired,
	antState_Error
};


struct antennaStruct
{
	enum antennaState State;
	struct geoLocationStruct GeoLocation;
	struct antennaAxis AzimuthAxis;
	struct antennaAxis ElevationAxis;
	struct antennaAttitudeStruct ReflectorAttitude;
	struct antennaAttitudeStruct BaseAttitude;
	struct satelliteStruct TargetSatellite;
	struct commsParameters CommSystem;
};
struct antennaStruct createantennaStruct();
#ifdef __USINGCONSOLEMENU
struct uiStruct; // forward declaration
struct logStruct; // forward declaration
struct configStruct; // forward declaration
void parseTerminalMenuAPI(struct antennaStruct* terminalStructPtrIn, struct uiStruct* uiStructPtrIn);
void writeTerminalMenuScreen(struct antennaStruct* terminalStructPtrIn, struct uiStruct* uiStructPtrIn);
void writeTerminalLogLine(struct antennaStruct* terminalStructPtrIn, struct logStruct* logStructPtrIn);
void parseTerminalConfigLine(struct antennaStruct* terminalStructPtrIn, struct configStruct* configStructPtrIn);

#endif

/////////////////////////////////////////////////////////////////////////////////////
// Functions
UI_8 CalculateLookAngle(struct antennaStruct* antStructInPtr);
UI_8 CalculatePedestalCoords(struct antennaStruct* antStructInPtr);
UI_8 CalculateWorldCoords(struct antennaStruct* antStructInPtr);

#endif // !__SATCOMCONTROL
