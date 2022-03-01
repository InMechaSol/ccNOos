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
    GPSserialdev.numBytes2Read = 1;
    eCompserialdev.numBytes2Read = 1;
    LCDKeyPadserialdev.readIndex = 0;
    ConsoleMenuserialdev.readIndex = 0;
    LCDKeyPadserialdev.numBytes2Read = 1;
    ConsoleMenuserialdev.numBytes2Read = 1;
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