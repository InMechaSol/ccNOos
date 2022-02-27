/** \file Application_Solution.h
*   \brief Cross-Platform Portable ccNOos SatComACS Example Declarations

   Copyright 2021 InMechaSol, Inc

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

Notes:

This header demonstrates the usage of the ccNOos library and its dependence
on other files in the library.  A platform specific main(.c,.cpp) file need
only include this single header to implement a platform specific ccNOos SatComACS
application.

*/
#ifndef __CCNOOS_SATCOMACS__
#define __CCNOOS_SATCOMACS__  

// Base ccNOos includes
#include "execution_system.h"
#include "console_menu.h"

// SatComACS and Device includes
#include "satComControl.h"
#include "APTmodule.h"
#include "TPMmodule.h"
#include "TxRxmodule.h"

// The main SatComACS module
#define Mn SatComACS



// Main SatComACS Data Structure
MODdeclareSTRUCT(Mn)
{
    COMPMODFIRST;
    // satComACS Terminal Module
    struct antennaStruct Terminal;
    // satComACS Device Modules
    struct aptStruct APT;
    struct wmmStruct WMM;
    struct tpmStruct TPM;    
    struct txRxStruct TxRx;    
    // satComACS API Device Modules
    struct uiStruct LCDKeyPad;
    struct uiStruct ConsoleMenu;
};

#define MODdeclareCREATEINS 
#define MODcallCREATEINS 

MODdeclareCREATE(Mn)(MODdeclareCREATEINS);

// Re-usable, portable, cross-platform (ccNOosTests setup() function)
MODdeclareSETUP(Mn);
// Re-usable, portable, cross-platform (ccNOosTests loop() function)
MODdeclareLOOP(Mn);
// Re-usable, portable, cross-platform (ccNOosTests systick() function)
MODdeclareSYSTICK(Mn);

#ifdef __USINGCONSOLEMENU 

enum currentMenuAction
{
    cA_Enter,
    cA_Up,
    cA_Down,
    cA_Forward,
    cA_Back,
    cA_Status,
    cA_Acquire
};

enum currentMenuNode
{
    cM_RootNode,
    cM_MainMenu=0,
    cM_Devices,
    cM_Terminal,
    cM_ExecutionSystem,
    cM_devTXRX,
    cM_devAPT,
    cM_devTPM
};

//// Module Console Menu Print and Parse
MODdeclarePRINTm(Mn);
MODdeclarePARSEi(Mn);

void linkAPIioDevices(struct SatComACSStruct* satcomacsStructPtrIn);

void parseSatComACSDevicesMenuAPI(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn);
void writeSatComACSDevicesMenuScreen(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn);

void parseSatComACSMenuAPI(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn);
void writeSatComACSMenuScreen(struct SatComACSStruct* satcomacsStructPtrIn, struct uiStruct* uiStructPtrIn);
void writeSatComACSLogLine(struct SatComACSStruct* satcomacsStructPtrIn, struct logStruct* logStructPtrIn);
void parseSatComACSConfigLine(struct SatComACSStruct* satcomacsStructPtrIn, struct configStruct* configStructPtrIn);


//// Module "JSON" serialization/deserialization for satComControl.h and motionControl.h data structures
//struct satelliteStruct createSatelliteStructJSON(char* JSONptrIn);
//void createJSONSatelliteStruct(struct satelliteStruct* satelliteStructPtrIn);
//struct commsParameters createcommsParametersJSON(char* JSONptrIn);
//void createJSONcommsParameters(struct commsParameters* commsParametersPtrIn);
//struct geoLocationStruct creategeoLocationStructJSON(char* JSONptrIn);
//void createJSONgeoLocationStruct(struct geoLocationStruct* geoLocationStructPtrIn);
//struct antennaAttitudeStruct createantennaAttitudeStructJSON(char* JSONptrIn);
//void createJSONantennaAttitudeStruct(struct antennaAttitudeStruct* antennaAttitudeStructPtrIn);
//struct antennaAxis createantennaAxisJSON(char* JSONptrIn);
//void createJSONantennaAxis(struct antennaAxis* antennaAxisPtrIn);
//struct antennaStruct createantennaStructJSON(char* JSONptrIn);
//void createJSONantennaStruct(struct antennaStruct* antennaStructPtrIn);
//struct axisStruct createaxisStructJSON(char* JSONptrIn);
//void createJSONaxisStruct(struct axisStruct* axisStructPtrIn);

#else
#error Console Required for SatComACS Module
#endif


////////////////////////////////////////////////////////////////////////////////
// C ccNOosTests Example Application - built from computeModuleClass and Execution System
#define __PLATFORM_APP_CTEMPLATE(MODNAME) \
    struct linkedEntryPointStruct setupListHead = {\
        nullptr,\
        (struct computeModuleStruct*)&MODdataINST(Mn),\
        MODsetup(Mn)\
        };\
    struct linkedEntryPointStruct loopListHead = {\
        nullptr,\
        (struct computeModuleStruct*)&MODdataINST(Mn),\
        MODloop(Mn)\
        };\
    struct executionEntryStruct exeEntryPoints = {\
        &setupListHead,\
        &loopListHead,\
        nullptr,\
        &setupListHead\
        };\
    void applicationConfig()\
    {\
        exeSystem = CreateExecutionSystemStruct(uSEC_PER_CLOCK);\
        MODdataINST(Mn) = MODstructCREATE(Mn)();\
    }
#define  PLATFORM_APP_CTEMPLATE(MODNAME) __PLATFORM_APP_CTEMPLATE(MODNAME)

#ifdef __cplusplus
////////////////////////////////////////////////////////////////////////////////
// C++ ccNOosTests Example Class - built from computeModuleClass
class MODCLASS_NAME(Mn) : public computeModuleClass {
protected:
    MODdeclareDATA(Mn);
public:
    MODCLASS_NAME(Mn)(MODdeclareCREATEINS);
    MODCLASS_SETUP_INLINE(Mn);
    MODCLASS_LOOP_INLINE(Mn);
    MODCLASS_SYSTICK_INLINE(Mn);
    MODCLASS_ExcpHndlr_INLINE(Mn);
};

////////////////////////////////////////////////////////////////////////////////
// C++ ccNOosTests Example Application - built from computeModuleClass and Execution System
#define __PLATFORM_APP_CLASS(MODNAME) class theApplicationClass{\
    public:\
    linkedEntryPointClass setupListHead;\
    linkedEntryPointClass loopListHead;\
    linkedEntryPointClass systickListHead;\
    linkedEntryPointClass exceptionListHead;\
    MODCLASS_NAME(MODNAME) MODNAME##CompMod;\
    executionSystemClass* theExecutionSystemPtr;\
    theApplicationClass() :\
        MODNAME##CompMod(),\
        setupListHead(& MODNAME##CompMod, nullptr),\
        loopListHead(& MODNAME##CompMod, nullptr),\
        systickListHead(nullptr, nullptr),\
        exceptionListHead(&MODNAME##CompMod, nullptr)\
    {\
        theExecutionSystemPtr = &exeSystem;\
        theExecutionSystemPtr->LinkTheListsHead(\
            &setupListHead,\
            &loopListHead,\
            &systickListHead,\
            &exceptionListHead\
        );\
    }\
}
#define PLATFORM_APP_CLASS(MODNAME) __PLATFORM_APP_CLASS(MODNAME)


#endif // !__cplusplus
#endif // !__CCNOOS_SATCOMACS__
