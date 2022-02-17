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

// SatComACS and Device includes (straight c linking) 
#ifdef __cplusplus
extern "C" {
#endif
#include "satComControl.h"
#include "NEO_LEA_M8T.h"
#include "HMR3300.h"
#include "ADRF6650.h"
#include "LTC2360.h" 
#ifdef __cplusplus
}
#endif


// The main SatComACS module
#define Mn SatComACS

// Tx/Rx Module Data Structure
#define MAX_NUM_ATTENUATORS (3u)
#define MIN_ATTEN_VAL (0.0)
#define MAX_ATTEN_VAL (31.75)
struct txRxStruct
{
    UI_8 CMD_AttenuatorBits, INDEX_Attenuator;
    float AttenuatorValues[MAX_NUM_ATTENUATORS];
    UI_8 AttenuatorNeedsWriting[MAX_NUM_ATTENUATORS];
};
struct txRxStruct createtxRxStruct();
void writeAttenuatorValues(struct txRxStruct* txRxStructPtrIn);

// APT Module Data Structure
struct gpsStruct
{
    struct devicedatastruct* devptr;
    struct gpsData data;
    UI_8 newGPSData;
};
struct gpsStruct creategpsStruct();
UI_8 readGPS(struct gpsStruct* gpsStructPtrIn);

struct eCompStruct
{
    struct devicedatastruct* devptr;
    struct eCompDataStruct data;
    UI_8 neweCompassData;
};
struct eCompStruct createeCompStruct();
UI_8 readEcompass(struct eCompStruct* eCompStructPtrIn);

struct aptStruct
{
    struct gpsStruct GPS;
    struct eCompStruct eCompass;    
};
struct aptStruct createaptStruct();
void tryReadAPTData(struct aptStruct* aptStructPtrIn);

// WMM Data Structure
struct wmmStruct
{
    float magdeclination;
    // save non-volatile...
    float lastGoodThreshold; // meters
    float lastGoodMultiplier; // deg per meter
    float lastGoodlatitude;
    float lastGoodlongitude;
    float lastGoodaltitude;
    float lastGoodmagdeclination;
};
struct wmmStruct createwmmStruct();

// TPM Module Data Structure
struct freqConvStruct
{
    struct devicedatastruct* devptr;
    struct ADRF6650DataStruct data;
    UI_8 newFreqConvData;    
};
struct freqConvStruct createfreqConvStruct();
void readFreqConv(struct freqConvStruct* freqConvStructPtrIn);
void writeFreqConv(struct freqConvStruct* freqConvStructPtrIn);

struct powerMeterStruct
{
    struct devicedatastruct* devptr;    
    struct LTC2360DataStruct data;
    UI_8 newPowerMeterValue;
};
struct powerMeterStruct createPowerMeterStruct();
void readPowerMeter(struct powerMeterStruct* powerMeterStructPtrIn);
void writePowerMeter(struct powerMeterStruct* powerMeterStructPtrIn);

struct tpmStruct
{
    struct freqConvStruct freqConverter;
    struct powerMeterStruct powerMeter;    
};
struct tpmStruct createtpmStruct();
void tryReadTPMData(struct tpmStruct* tpmStructPtrIn);
void tryWriteTPMData(struct tpmStruct* tpmStructPtrIn);



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
    // satComACS API Device Module
    struct uiStruct LCDKeyPad;
    // 
    UI_8 configured;
    UI_16 printed, parsed;
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

// Module Console Menu Print and Parse
MODdeclarePRINTm(Mn);
MODdeclarePARSEi(Mn);

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
private:
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
