/** \file TPMmodule.c
*   \brief Cross-Platform Portable ccNOos Tests Definitions

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

This source demonstrates the usage of the ccNOos library and its dependence
on other files in the library.  A platform specific main(.c,.cpp) file need
only instantiate, configure, then execute the execution system.

*/
#include "TPMmodule.h"


struct freqConvStruct createfreqConvStruct()
{
    struct freqConvStruct outStruct;
    outStruct.data = CreateADRF6650Data();    
    outStruct.newFreqConvData = ui8FALSE;   
    return outStruct;
}
struct powerMeterStruct createPowerMeterStruct()
{
    struct powerMeterStruct outStruct;
    outStruct.data.BandwidthMHz = 0;
    outStruct.newPowerMeterValue = ui8FALSE;
    outStruct.data.PowerMeasuredinBanddB = 0;
    outStruct.data.current_value = 0;
    outStruct.data.value_good = ui8FALSE;
    return outStruct;
}
struct tpmStruct createtpmStruct()
{
    struct tpmStruct outStruct;
    outStruct.freqConverter = createfreqConvStruct();
    outStruct.powerMeter = createPowerMeterStruct();
    return outStruct;
}

void tryReadTPMData(struct tpmStruct* tpmStructPtrIn)
{
    readFreqConv(&tpmStructPtrIn->freqConverter);
    readPowerMeter(&tpmStructPtrIn->powerMeter);
}

void tryWriteTPMData(struct tpmStruct* tpmStructPtrIn)
{
    writeFreqConv(&tpmStructPtrIn->freqConverter);
    writePowerMeter(&tpmStructPtrIn->powerMeter);
}
