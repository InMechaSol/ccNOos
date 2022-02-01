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
#include "../../../executionSystem/execution_system.h"
#include "../../../consoleMenu/console_menu.h"
// SatComACS includes
#include "../../../ccLibs/acs/satComControl.h"

// The main SatComACS module
#define Mn SatComACS

#define charBuffMax 80

// Console UI Data Structure
struct uiStruct
{
    UI_8    charsRead, chars2Write;
    char    charbuff_In[charBuffMax];
    char    charbuff_Out[charBuffMax];
};
void readUIchars(struct uiStruct* uiStructPtrIn);
void writeUIchars(struct uiStruct* uiStructPtrIn);

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
void writeAttenuatorValues(struct txRxStruct* txRxStructPtrIn);

// APT Module Data Structure
struct gpsStruct
{
    float lattitude, longitude, altitude, utctime;
    int day, month, year;
    UI_8 newGPSData;
};
void readGPS(struct gpsStruct);
struct eCompStruct
{
    float yaw, pitch, roll;
    UI_8 neweCompassData;
};
void readEcompass(struct eCompStruct* eCompStructPtrIn);
struct aptStruct
{
    struct gpsStruct GPS;
    struct eCompStruct eCompass;    
};
void tryReadAPTData(struct aptStruct* aptStructPtrIn);

// TPM Module Data Structure
struct freqConvStruct
{
    float DesiredCenterFreqMHz;
    float RequiredLOFreqMHz;
    UI_8 LockedOnDesiredFrequency;
    UI_8 newLockedValue;
};
void readFreqConv(struct freqConvStruct* freqConvStructPtrIn);
void writeFreqConv(struct freqConvStruct* freqConvStructPtrIn);
struct powerMeterStruct
{
    float BandwidthMHz;
    float PowerMeasuredinBanddB;
    UI_32 PowerMeterValue;
    UI_8 newPowerMeterValue;
};
void readPowerMeter(struct powerMeterStruct* powerMeterStructPtrIn);
void writePowerMeter(struct powerMeterStruct* powerMeterStructPtrIn);
struct tpmStruct
{
    struct freqConvStruct freqConverter;
    struct powerMeterStruct powerMeter;    
};
void tryReadTPMData(struct tpmStruct* tpmStructPtrIn);
void tryWriteTPMData(struct tpmStruct* tpmStructPtrIn);

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

// Main SatComACS Data Structure
MODdeclareSTRUCT(Mn)
{
    COMPMODFIRST;
    struct antennaStruct Terminal;
    struct aptStruct APT;
    struct tpmStruct TPM;
    struct uiStruct LCDKeyPad;
    struct txRxStruct TxRx;
    struct wmmStruct WMM;
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
MODdeclarePRINTm(Mn);
MODdeclarePARSEi(Mn);
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
