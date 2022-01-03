#ifndef __SATCOMCONTROL
#define __SATCOMCONTROL


#include "../../executionSystem/version_config.h"


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

struct commsParameters
{
	float Rx_LO;
};

struct antennaAxisStruct
{
	float CommandPosition;
	float FeedbackPosition;
	float PositionError;
};

struct geoLocationStruct
{
	float latitude;
	float longitude;
	float altitude;
};

struct antennaAttitudeStruct
{
	float roll;
	float pitch;
	float yaw;
};

enum antennaState
{
	antState_init,
	antState_NotAcquired,
	antState_Acquiring,
	antState_Acquired,
	antState_Error
};

struct antennaStuct
{
	enum antennaState State;
	struct geoLocationStruct GeoLocation;
	struct antennaAxisStruct AzimuthAxis;
	struct antennaAxisStruct ElevationAxis;
	struct antennaAttitudeStruct BaseAttitude;
};

/////////////////////////////////////////////////////////////////////////////////////
// Functions
UI_8 CalculateLookAngle(struct antennaStruct *antStructInPtr)
{
	return ui8FALSE;
}

#endif // !__SATCOMCONTROL