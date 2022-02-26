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

void readTerminalMenuAPI(struct antennaStruct* terminalStructPtrIn, struct uiStruct* uiStructPtrIn);
void writeTerminalMenuScreen(struct antennaStruct* terminalStructPtrIn, struct uiStruct* uiStructPtrIn);
void writeTerminalLogLine(struct antennaStruct* terminalStructPtrIn, struct logStruct* logStructPtrIn);
void readTerminalConfigLine(struct antennaStruct* terminalStructPtrIn, struct configStruct* configStructPtrIn);

#endif

/////////////////////////////////////////////////////////////////////////////////////
// Functions
UI_8 CalculateLookAngle(struct antennaStruct* antStructInPtr);
UI_8 CalculatePedestalCoords(struct antennaStruct* antStructInPtr);
UI_8 CalculateWorldCoords(struct antennaStruct* antStructInPtr);

#endif // !__SATCOMCONTROL
