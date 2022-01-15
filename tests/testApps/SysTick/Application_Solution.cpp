///////////////////////////////////////////////////////////////////////
// SysTick Example
///////////////////////////////////////////////////////////////////////
#ifndef COMPILE_TESTS
#ifdef EXAMPLE_SYSTICK

#include "Application_Solution.c"

MODCLASS_NAME(Mn)::MODCLASS_NAME(Mn)(MODdeclareCREATEINS) :
    computeModuleClass(&MODdataINST(Mn).compMod)
{
    MODdataINST(Mn) = MODstructCREATE(Mn)(MODcallCREATEINS);
}
#endif
#endif