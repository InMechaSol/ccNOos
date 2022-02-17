#ifndef NEOLEAM8T_H__
#define NEOLEAM8T_H__

#include "console_menu.h"


struct gpsData
{
	float lattitude, longitude, altitude, utctime;
	I_16 day, month, year;
};

struct gpsData createGPSDataStruct();

UI_8 tryParseGPSData(char* gpsStringin, struct gpsData* gpsDataPtr);


#endif  // NEOLEAM8T_H__
