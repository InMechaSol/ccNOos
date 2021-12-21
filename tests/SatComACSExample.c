///////////////////////////////////////////////////////////////////////
// SatCom ACS Example
///////////////////////////////////////////////////////////////////////
#ifndef COMPILE_TESTS
#ifdef EXAMPLE_SATCOM_ACS

#include "SatComACSExample.h"

// Re-usable, portable, cross-platform (systick example create() function)
MODSTRUCT_CREATE_PROTO(MODULENAME)
{
    int i;
    MODSTRUCT(MODULENAME) outStruct;
    outStruct.compMod = CreateComputeModuleStruct();
   
    return outStruct;
}



// Re-usable, portable, cross-platform (systick example setup() function)
MODULE_FUNC_PROTO_SETUP(MODULENAME)
{
    MODDATAPTR_ERROR_RETURN(MODULENAME, AttenUIPtr);

    // Setup is running in the loop area to handle exceptions...
    IF_MODULE_ERROR(AttenUIPtr)
    {
        CLEAR_MODULE_ERRORS(AttenUIPtr);  // do nothing, clear flags
    }
    // Setup is running in the setup area following power on
    else
    {

    }
    return RETURN_SUCCESS;
}

// Re-usable, portable, cross-platform (systick example loop() function)
MODULE_FUNC_PROTO_LOOP(MODULENAME)
{
    MODDATAPTR_ERROR_RETURN(MODULENAME, AttenUIPtr);

    
    return RETURN_SUCCESS;
}

MODULE_FUNC_PROTO_SYSTICK(MODULENAME) { ; }  // do nothing in the systick area

#endif //!Attenuators UI Example
#endif