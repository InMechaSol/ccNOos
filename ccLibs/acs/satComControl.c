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
	return ui8FALSE;
}
