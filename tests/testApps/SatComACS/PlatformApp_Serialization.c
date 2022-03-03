/** \file PlatformApp_Serialization.c
*   \brief part of <a href="https://github.com/InMechaSol/ccNOos">ccNOos</a>, Implementation for straight C 

    Copyright 2021 InMechaSol, Inc

    https://www.inmechasol.org/

    Licensed under the Apache License, Version 2.0(the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

Notes:
	(.c includes .h) - for straight C or
	(.cpp includes .c which includes .h) - for C++ wrapped straight C
	*Always compiled to a single compilation unit, either C or CPP, not both

*/

#include "PlatformApp_Serialization.h"

// SatComACS
void writeSatComACSLogLine(struct SatComACSStruct* satcomacsStructPtrIn, struct logStruct* logStructPtrIn) { ; }
void parseSatComACSConfigLine(struct SatComACSStruct* satcomacsStructPtrIn, struct configStruct* configStructPtrIn) { ; }
// SatComControl: antennaStruct
void writeTerminalLogLine(struct antennaStruct* terminalStructPtrIn, struct logStruct* logStructPtrIn) { ; }
void parseTerminalConfigLine(struct antennaStruct* terminalStructPtrIn, struct configStruct* configStructPtrIn) { ; }
// TxRx Module
void writeTxRxLogLine(struct txRxStruct* txRxStructPtrIn, struct logStruct* logStructPtrIn) { ; }
void parseTxRxConfigLine(struct txRxStruct* txRxStructPtrIn, struct configStruct* configStructPtrIn) { ; }
// APT Module
void writeAPTLogLine(struct aptStruct* aptStructPtrIn, struct logStruct* logStructPtrIn) { ; }
void parseAPTConfigLine(struct aptStruct* aptStructPtrIn, struct configStruct* configStructPtrIn) { ; }
// TPM Module
void writeTPMLogLine(struct tpmStruct* tpmStructPtrIn, struct logStruct* logStructPtrIn) { ; }
void parseTPMConfigLine(struct tpmStruct* tpmStructPtrIn, struct configStruct* configStructPtrIn) { ; }



//struct satelliteStruct createSatelliteStructJSON(char* JSONptrIn)
//{
//
//}
//void createJSONSatelliteStruct(struct satelliteStruct* satelliteStructPtrIn)
//{
//
//}
//struct commsParameters createcommsParametersJSON(char* JSONptrIn)
//{
//
//}
//void createJSONcommsParameters(struct commsParameters* commsParametersPtrIn)
//{
//
//}
//struct geoLocationStruct creategeoLocationStructJSON(char* JSONptrIn)
//{
//
//}
//void createJSONgeoLocationStruct(struct geoLocationStruct* geoLocationStructPtrIn)
//{
//
//}
//struct antennaAttitudeStruct createantennaAttitudeStructJSON(char* JSONptrIn)
//{
//
//}
//void createJSONantennaAttitudeStruct(struct antennaAttitudeStruct* antennaAttitudeStructPtrIn)
//{
//
//}
//struct antennaAxis createantennaAxisJSON(char* JSONptrIn)
//{
//
//}
//void createJSONantennaAxis(struct antennaAxis* antennaAxisPtrIn)
//{
//
//}
//struct antennaStruct createantennaStructJSON(char* JSONptrIn)
//{
//
//}
//void createJSONantennaStruct(struct antennaStruct* antennaStructPtrIn)
//{
//
//}