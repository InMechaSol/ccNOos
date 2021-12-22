///////////////////////////////////////////////////////////////////////
// Attenuators UI Example
///////////////////////////////////////////////////////////////////////
#ifndef COMPILE_TESTS
#ifdef EXAMPLE_ATTEN_UI

#include "AttensUIExample.c"

MODCLASS_NAME(Mn)::MODCLASS_NAME(Mn)(MODdeclareCREATEINS) :
    computeModuleClass(&MODdataINST(Mn).compMod)
{
    MODdataINST(Mn) = MODstructCREATE(Mn)(MODcallCREATEINS);
}
#endif
#endif