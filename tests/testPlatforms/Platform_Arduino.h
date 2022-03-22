/** \file Platform_Arduino.h
*   \brief <a href="https://www.inmechasol.org/" target="_blank">IMS</a>:
		<a href="https://github.com/InMechaSol/ccNOos" target="_blank">ccNOos</a>,
		Platform Specification, Arduino 

    Copyright 2021 <a href="https://www.inmechasol.org/" target="_blank">InMechaSol, Inc</a>    

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

#if PLATFORM_NAME!=Arduino
error PLATFORM_NAME must be Arduino
#endif


#ifdef REDEFINE_NULLPTR
#error Must not compile with -DREDEFINE_NULLPTR on Aduino ATMega (maybe others, need to test)
#endif // !REDEFINE_NULLPTR
#ifndef __NOEXCEPTIONS
#error Must compile with -D__NOEXCEPTIONS on Aduino ATMega (maybe others, need to test)
#endif // !__NOEXCEPTIONS


#include <Arduino.h>
#include <Adafruit_MCP23017.h>
#include "execution_system.h"    
#include "console_menu.h" 
#define LIGHT_OFF (0u)                          // 1-PSoC4, 0-most others
#define uSEC_PER_CLOCK (1000u)

struct SerialDeviceStruct GPSserialdev;
struct SerialDeviceStruct eCompserialdev;
struct SerialDeviceStruct LCDKeyPadserialdev;
struct SerialDeviceStruct ConsoleMenuserialdev;

int gpsCharInt = -1;
int eCompCharInt = -1;
int consoleCharInt = -1;
int lcdCharInt = -1;
int idx = 0;
void ReadSerialUARTS()
{
    consoleCharInt = Serial.read();
    gpsCharInt = Serial1.read();
    eCompCharInt = Serial2.read();
}


#define IC1Addr 0x27
#define IC12Addr 0x00
Adafruit_MCP23017 IC1;
Adafruit_MCP23017 IC12;


// 0) (Optional) Platform Config and Log Files/Devices
// 1) Platform Setup Function
void platformSetup()
{
    //<platformSetup>
#ifdef __USINGCONSOLEMENU
#ifdef __USINGFLOATPRINTF
    asm(".global _printf_float");
#endif
#endif
    ConsoleMenuserialdev.devdata = createDeviceStruct();
    ConsoleMenuserialdev.baudRate = 115200;
    ConsoleMenuserialdev.dataBits = 8;
    ConsoleMenuserialdev.stopBits = stop_one;
    ConsoleMenuserialdev.parity = parity_none;
    ConsoleMenuserialdev.handshake = handshake_none;

    LCDKeyPadserialdev.devdata = createDeviceStruct();
    LCDKeyPadserialdev.baudRate = 9600;
    LCDKeyPadserialdev.dataBits = 8;
    LCDKeyPadserialdev.stopBits = stop_one;
    LCDKeyPadserialdev.parity = parity_none;
    LCDKeyPadserialdev.handshake = handshake_none;

    GPSserialdev.devdata = createDeviceStruct();
    GPSserialdev.baudRate = 9600;
    GPSserialdev.dataBits = 8;
    GPSserialdev.stopBits = stop_one;
    GPSserialdev.parity = parity_none;
    GPSserialdev.handshake = handshake_none;

    eCompserialdev.devdata = createDeviceStruct();
    eCompserialdev.baudRate = 19200;
    eCompserialdev.dataBits = 8;
    eCompserialdev.stopBits = stop_one;
    eCompserialdev.parity = parity_none;
    eCompserialdev.handshake = handshake_none;

    Serial.begin(115200);
    Serial1.begin(GPSserialdev.baudRate);
    Serial2.begin(eCompserialdev.baudRate);
    Serial3.begin(LCDKeyPadserialdev.baudRate);
    Wire.begin();

    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    //</platformSetup>
}
// 2) Platform Start Function
void platformStart()
{
    //<platformStart>
    GPSserialdev.readIndex = 0;
    eCompserialdev.readIndex = 0;
    GPSserialdev.devdata.numbytes2Read = 1;
    eCompserialdev.devdata.numbytes2Read = 1;
    LCDKeyPadserialdev.readIndex = 0;
    ConsoleMenuserialdev.readIndex = 0;
    LCDKeyPadserialdev.devdata.numbytes2Read = 1;
    ConsoleMenuserialdev.devdata.numbytes2Read = 1;
    
    //</platformStart>
}
// 3) Platform Loop Delay Function
void platformLoopDelay()
{

    ReadSerialUARTS();


    //<platformLoopDelay>
    ;
    //</platformLoopDelay>
}

#ifdef __USINGCONSOLEMENU

// 8) Platform API Functions (From Template?)
PlatformAPIFuncsTemplate(size + 1);

#endif
// 9) Global Execution System Instance
//executionSystemClass PLATFORM_EXESYS_NAME(PLATFORM_NAME)(uSEC_PER_CLOCK);
// 10) ExeSys API Functions (From Template?)
ExeSysAPIFuncsTemplate