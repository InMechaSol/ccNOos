#include "ADRF6650.h"

struct ADRF6650DataStruct CreateADRF6650Data()
{
	struct ADRF6650DataStruct outStruct;
	outStruct.DesiredCenterFreqMHz = 0;
	outStruct.LockedOnRequiredLOFreq = ui8FALSE;
	outStruct.RequiredLOFreqMHz = 0;
	return outStruct;
}

