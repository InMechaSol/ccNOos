#include "motionControl.h"

struct axisStruct createaxisStruct()
{
	struct axisStruct outStruct;
	outStruct.PositionCmd = 0;
	outStruct.PositionFbk = 0;
	outStruct.PositionErr = 0;
	return outStruct;
};