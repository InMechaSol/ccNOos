/** \file satComControl.c
*   \brief part of ccNOos, Implementation for straight C, or Declarations for C++ wrappers 

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

#include "satComControl.h"

struct satelliteStruct createSatelliteStruct()
{
	struct satelliteStruct outStruct;
	outStruct.UpLinkFreq_kHz = 0;
	outStruct.DownLinkFreq_kHz = 0;
	outStruct.SSP_Lattitude = 0;
	outStruct.SSP_Longitude = 0;
	outStruct.SSP_Altitude = 0;	
	return outStruct;
}
struct commsParameters createcommsParameters()
{
	struct commsParameters outStruct;
	outStruct.Rx_LO_MHz = 0;
	outStruct.Tx_LO_MHz = 0;
	outStruct.RxSignalStrengthMetric = 0;
	return outStruct;
}
struct geoLocationStruct creategeoLocationStruct()
{
	struct geoLocationStruct outStruct;
	outStruct.altitude = 0;
	outStruct.latitude = 0;
	outStruct.longitude = 0;
	return outStruct;
}
struct antennaAttitudeStruct createantennaAttitudeStruct()
{
	struct antennaAttitudeStruct outStruct;
	outStruct.pitch = 0;
	outStruct.roll = 0;
	outStruct.yaw = 0;
	return outStruct;
}
struct antennaAxis createantennaAxis()
{
	struct antennaAxis outStruct;
	outStruct.Pedestal = createaxisStruct();
	outStruct.World = createaxisStruct();
	return outStruct;
}
struct antennaStruct createantennaStruct()
{
	struct antennaStruct outStruct;
	outStruct.State = antState_init;
	outStruct.GeoLocation = creategeoLocationStruct();
	outStruct.AzimuthAxis = createantennaAxis();
	outStruct.ElevationAxis = createantennaAxis();
	outStruct.ReflectorAttitude = createantennaAttitudeStruct();
	outStruct.BaseAttitude = createantennaAttitudeStruct();
	outStruct.TargetSatellite = createSatelliteStruct();
	outStruct.CommSystem = createcommsParameters();
	return outStruct;
}
UI_8 CalculateLookAngle(struct antennaStruct* antStructInPtr)
{
    if(antStructInPtr->State)
    {
        ;
    }
	return ui8FALSE;
}
