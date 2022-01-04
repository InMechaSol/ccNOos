///////////////////////////////////////////////////////////////////////
// SatCom Tunable Power Meter Example
///////////////////////////////////////////////////////////////////////
#ifndef COMPILE_TESTS
#ifdef EXAMPLE_POWER_METER

#include "SatComPowerMeterExample.c"

MODCLASS_NAME(Mn)::MODCLASS_NAME(Mn)(MODdeclareCREATEINS) :
    computeModuleClass(&MODdataINST(Mn).compMod)
{
    MODdataINST(Mn) = MODstructCREATE(Mn)(MODcallCREATEINS);
}
#endif
#endif