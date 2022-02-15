#ifndef HMR3300_H
#define HMR3300_H

#include "version_config.h"


struct eCompDataStruct
{
	float yaw, pitch, roll;
};
struct eCompDataStruct createEcompDataStruct();

UI_8 tryParseEcompData(char* eCompStringin, struct eCompDataStruct* eCompDataPtr);

#endif // HMR3300_H
