#ifndef __SATCOMCONTROL
#define __SATCOMCONTROL


#include "../mcs/motionControl.h"


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
	float Rx_LO_MHz;
	float Tx_LO_MHz;
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
struct antennaAxis
{
	struct axisStruct World;
	struct axisStruct Pedestal;
};
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
};

/////////////////////////////////////////////////////////////////////////////////////
// Functions
struct antennaStruct CreateAntennaStruct();
UI_8 CalculateLookAngle(struct antennaStruct* antStructInPtr);
UI_8 CalculatePedestalCoords(struct antennaStruct* antStructInPtr);

#endif // !__SATCOMCONTROL
