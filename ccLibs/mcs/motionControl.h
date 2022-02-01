#ifndef __ccNOos_MCS__
#define __ccNOos_MCS__

#include "../../executionSystem/version_config.h"

struct axisStruct
{
	float PositionCmd;
	float PositionFbk;
	float PositionErr;
};
struct axisStruct createaxisStruct();

#endif