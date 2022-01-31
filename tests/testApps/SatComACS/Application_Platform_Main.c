/** \file WIN32_Console.cpp
*   \brief WIN32 Console Implementation of ccNOos_Tests in c++

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

*/

// Include all things ccNOos_tests, which is an application of ccNOos
// ccNOos_tests is the application solution
#ifdef COMPILE_TESTS
#include "..\testApps\ccNOos_Tests\PlatformApp_Serialization.h"
#else // !COMPILE_TESTS
#ifdef EXAMPLE_SYSTICK
#include "..\testApps\SysTick\PlatformApp_Serialization.h"
#endif // EXAMPLE_SYSTICK
#ifdef EXAMPLE_SATCOM_ACS
#include "..\testApps\SatComACS\PlatformApp_Serialization.h"
#endif // EXAMPLE_SATCOM_ACS
#endif // !COMPILE_TESTS


////////////////////////////////
// Compile Error if Examples/Tests not defining 
#ifndef Mn
    #error Mn must be defined - see examples
#endif
#ifndef MODdeclareCREATEINS
    #error MODdeclareCREATEINS must be defined - see examples
#endif
#ifndef MODcallCREATEINS
    #error MODcallCREATEINS must be defined - see examples
#endif
#ifdef __cplusplus
    #ifndef PLATFORM_APP_CLASS
        #error PLATFORM_APP_CLASS macro must be defined in application code
    #endif
#else
    #ifndef MODdeclareDATA
        #error MODdeclareDATA macro must be defined in application code
    #endif
#endif

////////////////////////////////
// Compile Error if Examples/Tests not defining
#if PLATFORM_NAME==PSoC4
#include <Platform_PSoC4.h>
#elif PLATFORM_NAME==Win32
#include "Platform_Win32.h"
#elif PLATFORM_NAME==QTCreatorC
#include <Platform_QTCreatorC.h>
#else
#error "No Platform Selected for Compile!"
#endif


///////////////////////////////////////////////////////////////////////
// Module Data Structure or Module Data Class 
//   -Declared from Macro Template
//   -Macro Template is Defined in the Application Solution Header
///////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
    PLATFORM_APP_CLASS(Mn);
#else
    MODdeclareDATA(Mn);
#endif









#ifndef COMPILE_TESTS
///////////////////////////////////////////////////////////////////////
// SysTick Example
///////////////////////////////////////////////////////////////////////
#ifdef EXAMPLE_SYSTICK
#if PLATFORM_NAME==PSoC4
    // 1) Minute LED Device Write
    void WriteMinLED(MODdeclarePTRIN(Mn))
    {
        //<writeMinLEDdevice>
        LED_Min_Write(MODdataPTR(Mn)->MinLEDvalue); 
        //</writeMinLEDdevice>
    }
    // 2) Second LED Device Write
    void WriteSecLED(MODdeclarePTRIN(Mn))
    {
        //<writeSecLEDdevice>
        LED_Sec_Write(MODdataPTR(Mn)->SecLEDvalue);
        //</writeSecLEDdevice>
    }
    // 3) Serial Device Write
    void WriteTimeSerial(MODdeclarePTRIN(Mn))
    {
        //<writeSerialdevice>
        UART_PutString(MODdataPTR(Mn)->time); 
        //</writeSerialdevice>
    }
#elif PLATFORM_NAME==Win32
    // 1) Minute LED Device Write
    void WriteMinLED(MODdeclarePTRIN(Mn)){ ; }
    // 2) Second LED Device Write
    void WriteSecLED(MODdeclarePTRIN(Mn)) { ; }
    // 3) Serial Device Write
    void WriteTimeSerial(MODdeclarePTRIN(Mn))
    {
        std::cout << MODdataPTR(Mn)->time;
    }
#elif PLATFORM_NAME==QTCreatorC
    // 1) Minute LED Device Write
    void WriteMinLED(MODdeclarePTRIN(Mn)) { ; }
    // 2) Second LED Device Write
    void WriteSecLED(MODdeclarePTRIN(Mn)) { ; }
    // 3) Serial Device Write
    void WriteTimeSerial(MODdeclarePTRIN(Mn))
    {
        printf("%s", MODdataPTR(Mn)->time);
        fflush(stdout);
    }
#elif PLATFORM_NAME==Arduino
    // 1) Minute LED Device Write
    void WriteMinLED(MODdeclarePTRIN(Mn)) { ; }
    // 2) Second LED Device Write
    void WriteSecLED(MODdeclarePTRIN(Mn)) { ; }
    // 3) Serial Device Write
    void WriteTimeSerial(MODdeclarePTRIN(Mn))
    {
        Serial.write("\r");
        Serial.write(MODdataPTR(Mn)->time);
        Serial.write("\n");
    }
#else
#error "No Platform Selected for EXAMPLE!"
#endif

#endif //!EXAMPLE_SYSTICK

///////////////////////////////////////////////////////////////////////
// Attenuators UI Example
///////////////////////////////////////////////////////////////////////
#ifdef EXAMPLE_ATTEN_UI
#if PLATFORM_NAME==Win32

    float ModuloFloat(float floatValue, float* intPartPtr)
    {
        return modf(floatValue, intPartPtr);
    }
    void WriteAttenuators(MODdeclarePTRIN(Mn))
    {
#define bit16   ( (0b10000000 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 7 )
#define bit8    ( (0b01000000 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 6 )
#define bit4    ( (0b00100000 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 5 )
#define bit2    ( (0b00010000 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 4 )
#define bit1    ( (0b00001000 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 3 )
#define bit0_25 ( (0b00000100 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 2 )
#define bit0_50 ( (0b00000010 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 1 )
        //float fracPart = (16.0 * bit16) + (8.0 * bit8) + (4.0 * bit4) + (2.0 * bit2) + (1.0 * bit1) + (0.25 * bit0_25) + (0.50 * bit0_50);
        //std::cout << fracPart;
#undef bit16  
#undef bit8
#undef bit4
#undef bit2
#undef bit1
#undef bit0_25 
#undef bit0_50 
}

#elif PLATFORM_NAME==QTCreatorC

    float ModuloFloat(float floatValue, float* intPartPtr)
    {
        double retVal;
        double dblValue = floatValue;
        double intPartPtrDb;
        retVal = modf(dblValue, &intPartPtrDb);
        *intPartPtr = (float)(intPartPtrDb);
        return (float)retVal;
    }
    // platform and application specific io device functions
    void WriteAttenuators(MODdeclarePTRIN(Mn))
    {
    #define bit16   ( (0b10000000 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 7 )
    #define bit8    ( (0b01000000 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 6 )
    #define bit4    ( (0b00100000 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 5 )
    #define bit2    ( (0b00010000 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 4 )
    #define bit1    ( (0b00001000 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 3 )
    #define bit0_25 ( (0b00000100 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 2 )
    #define bit0_50 ( (0b00000010 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 1 )
            //float fracPart = (16.0 * bit16) + (8.0 * bit8) + (4.0 * bit4) + (2.0 * bit2) + (1.0 * bit1) + (0.25 * bit0_25) + (0.50 * bit0_50);
            //std::cout << fracPart;
    #undef bit16
    #undef bit8
    #undef bit4
    #undef bit2
    #undef bit1
    #undef bit0_25
    #undef bit0_50
    }

#elif PLATFORM_NAME==Arduino

#define SETPIN_TX_C16(high_low)    IC1.digitalWrite(0, high_low)
#define SETPIN_TX_C8(high_low)     IC1.digitalWrite(1, high_low)
#define SETPIN_TX_C4(high_low)     IC1.digitalWrite(2, high_low)
#define SETPIN_TX_C2(high_low)     IC1.digitalWrite(3, high_low)
#define SETPIN_TX_C1(high_low)     IC1.digitalWrite(4, high_low)
#define SETPIN_TX_C0_25(high_low)  IC1.digitalWrite(5, high_low)
#define SETPIN_TX_C0_50(high_low)  IC1.digitalWrite(6, high_low)
#define SETPIN_TX_LE(high_low)     IC1.digitalWrite(14, high_low) 

#define SETPIN_RX_C16(high_low)    IC1.digitalWrite(7, high_low)
#define SETPIN_RX_C8(high_low)     IC1.digitalWrite(8, high_low)
#define SETPIN_RX_C4(high_low)     IC1.digitalWrite(9, high_low)
#define SETPIN_RX_C2(high_low)     IC1.digitalWrite(10, high_low)
#define SETPIN_RX_C1(high_low)     IC1.digitalWrite(11, high_low)
#define SETPIN_RX_C0_25(high_low)  IC1.digitalWrite(12, high_low)
#define SETPIN_RX_C0_50(high_low)  IC1.digitalWrite(13, high_low)
#define SETPIN_RX_LE(high_low)     IC1.digitalWrite(15, high_low) 

#define SETPIN_XX_C16(high_low)    
#define SETPIN_XX_C8(high_low)     
#define SETPIN_XX_C4(high_low)     
#define SETPIN_XX_C2(high_low)     
#define SETPIN_XX_C1(high_low)     
#define SETPIN_XX_C0_25(high_low)  
#define SETPIN_XX_C0_50(high_low)  
#define SETPIN_XX_LE(high_low)

float ModuloFloat(float floatValue, float* intPartPtr)
{
    double intPart, fracPart;
    fracPart = modf(floatValue, &intPart);
    *intPartPtr = (float)intPart;
    return (float)fracPart;
}
// Application Specific, Platform modifications
#define IC1Addr 0x27
bool runOnce = true;
Adafruit_MCP23017 IC1;
// platform and application specific io device functions
void WriteAttenuators(MODdeclarePTRIN(Mn))
{
    if (runOnce)
    {
        runOnce = false;
        IC1.begin(IC1Addr);
        IC1.pinMode(0, OUTPUT);
        IC1.pinMode(1, OUTPUT);
        IC1.pinMode(2, OUTPUT);
        IC1.pinMode(3, OUTPUT);
        IC1.pinMode(4, OUTPUT);
        IC1.pinMode(5, OUTPUT);
        IC1.pinMode(6, OUTPUT);
        IC1.pinMode(7, OUTPUT);
        IC1.pinMode(8, OUTPUT);
        IC1.pinMode(9, OUTPUT);
        IC1.pinMode(10, OUTPUT);
        IC1.pinMode(11, OUTPUT);
        IC1.pinMode(12, OUTPUT);
        IC1.pinMode(13, OUTPUT);
        IC1.pinMode(14, OUTPUT);
        IC1.pinMode(15, OUTPUT);
    }

#define bit16   ( (0b10000000 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 7 )
#define bit8    ( (0b01000000 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 6 )
#define bit4    ( (0b00100000 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 5 )
#define bit2    ( (0b00010000 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 4 )
#define bit1    ( (0b00001000 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 3 )
#define bit0_25 ( (0b00000100 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 2 )
#define bit0_50 ( (0b00000010 & MODdataPTR(Mn)->CMD_AttenuatorBits) >> 1 )

    switch (AttenUIDataPtrIn->INDEX_Attenuator) {
    case 0:
        SETPIN_TX_C16(bit16);
        SETPIN_TX_C8(bit8);
        SETPIN_TX_C4(bit4);
        SETPIN_TX_C2(bit2);
        SETPIN_TX_C1(bit1);
        SETPIN_TX_C0_25(bit0_25);
        SETPIN_TX_C0_50(bit0_50);
        SETPIN_TX_LE(0x01);
        delay(1);
        SETPIN_TX_LE(0x00);
        break;
    case 1:
        SETPIN_RX_C16(bit16);
        SETPIN_RX_C8(bit8);
        SETPIN_RX_C4(bit4);
        SETPIN_RX_C2(bit2);
        SETPIN_RX_C1(bit1);
        SETPIN_RX_C0_25(bit0_25);
        SETPIN_RX_C0_50(bit0_50);
        SETPIN_RX_LE(0x01);
        delay(1);
        SETPIN_RX_LE(0x00);
        break;
    case 2:
        SETPIN_XX_C16(bit16);
        SETPIN_XX_C8(bit8);
        SETPIN_XX_C4(bit4);
        SETPIN_XX_C2(bit2);
        SETPIN_XX_C1(bit1);
        SETPIN_XX_C0_25(bit0_25);
        SETPIN_XX_C0_50(bit0_50);
        SETPIN_XX_LE(0x01);
        delay(1);
        SETPIN_XX_LE(0x00);
        break;
    default:
        break;
    }

#undef bit16  
#undef bit8
#undef bit4
#undef bit2
#undef bit1
#undef bit0_25 
#undef bit0_50 
}

void WriteMenuLine(MODdeclarePTRIN(Mn))
{
    Serial.write(MODdataPTR(Mn)->consoleLine);
}

#else
#error "No Platform Selected for EXAMPLE!"
#endif





#endif //!EXAMPLE_ATTEN_UI

///////////////////////////////////////////////////////////////////////
// SatCom ACS Example
///////////////////////////////////////////////////////////////////////
#ifdef EXAMPLE_SATCOM_ACS
#if PLATFORM_NAME==Win32
    // 1) Minute LED Device Write
void WriteMinLED(MODdeclarePTRIN(Mn)) { ; }
// 2) Second LED Device Write
void WriteSecLED(MODdeclarePTRIN(Mn)) { ; }
// 3) Serial Device Write
void WriteTimeSerial(MODdeclarePTRIN(Mn))
{
    std::cout << MODdataPTR(Mn)->time;
}
#elif PLATFORM_NAME==QTCreatorC
    // 1) Minute LED Device Write
void WriteMinLED(MODdeclarePTRIN(Mn)) { ; }
// 2) Second LED Device Write
void WriteSecLED(MODdeclarePTRIN(Mn)) { ; }
// 3) Serial Device Write
void WriteTimeSerial(MODdeclarePTRIN(Mn))
{
    printf("%s", MODdataPTR(Mn)->time);
    fflush(stdout);
}
#else
#error "No Platform Selected for EXAMPLE!"
#endif
#endif //!EXAMPLE_SATCOM_ACS

///////////////////////////////////////////////////////////////////////
// SatCom Tunable Power Meter Example
///////////////////////////////////////////////////////////////////////
#ifdef EXAMPLE_POWER_METER
#if PLATFORM_NAME==Win32
// platform and application specific io device functions
void WriteSPIDevice(MODdeclarePTRIN(Mn))
{

}
void ReadSPIDevice(MODdeclarePTRIN(Mn))
{

}
void WriteChipSelect(MODdeclarePTRIN(Mn))
{

}
#elif PLATFORM_NAME==QTCreatorC
// platform and application specific io device functions
void WriteSPIDevice(MODdeclarePTRIN(Mn))
{

}
void ReadSPIDevice(MODdeclarePTRIN(Mn))
{

}
void WriteChipSelect(MODdeclarePTRIN(Mn))
{

}
#else
#error "No Platform Selected for EXAMPLE!"
#endif
#endif //!EXAMPLE_SATCOM_ACS
#endif // !COMPILE_TESTS









///////////////////////////////////////////////////////////////////////
// Application Data Instances are Created here (Platform Specific)
#ifdef __cplusplus
theApplicationClass theApplicationExample;
#else
PLATFORM_APP_CTEMPLATE(Mn)
#endif

////////////////////////////////////////////////////////////////////////////////
// Platform Main Entry Points call ExeSys Area Functions
#ifdef MAIN_C_NOos_Wsystick
C_NOos_MAINnSYSTICK_TEMPLATE
#endif
#ifdef MAIN_C_NOos_NOsystick
C_OS_MAIN_TEMPLATE
#endif
