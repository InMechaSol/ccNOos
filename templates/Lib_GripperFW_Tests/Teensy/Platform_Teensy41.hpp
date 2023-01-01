/** \file Platform_Teensy41.hpp
*   \brief <a href="https://www.inmechasol.org/" target="_blank">IMS</a>:
		<a href="https://github.com/InMechaSol/ccNOos" target="_blank">ccNOos</a>,
		Platform Specification, Teensy Arduino 

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




#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>
#include "execution_system.h"    
#include "console_menu.h" 
#define LIGHT_OFF (0u)                          // 1-PSoC4, 0-most others
#define uSEC_PER_CLOCK (1000u)

#if PLATFORM_NAME!=Arduino
error PLATFORM_NAME must be Arduino
#endif


#ifdef REDEFINE_NULLPTR
#error Must not compile with -DREDEFINE_NULLPTR on Arduino Teensy (maybe others, need to test)
#endif // !REDEFINE_NULLPTR
#ifndef __NOEXCEPTIONS
#error Must compile with -D__NOEXCEPTIONS on Arduino Teensy (maybe others, need to test)
#endif // !__NOEXCEPTIONS


int consoleCharInt = -1;
int motor0CharInt = -1;
int motor1CharInt = -1;
int motor2CharInt = -1;
int motor3CharInt = -1;
int udpByteInt = -1;

EthernetUDP Udp;
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xF0, 0x0D
};
IPAddress ip(192, 168, 2, 254);
//IPAddress linuxIp(192, 168, 2, 2);
unsigned int localPort = 8888;      // local port to listen on
#define C_DEST_UDP_PORT 8889

// 0) (Optional) Platform Config and Log Files/Devices
// 1) Platform Setup Function
void platformSetup()
{
    
#ifdef __USINGCONSOLEMENU
#ifdef __USINGFLOATPRINTF
    asm(".global _printf_float");
#endif
#endif
    Serial1.begin(115200);
    Serial2.begin(115200);
    Serial3.begin(115200);
    Serial4.begin(115200);
    Serial.begin(115200);

    Ethernet.begin(mac, ip); // establish the default IP address and MAC for the gripper
    
    
    Udp.begin(localPort);
    //while (!Serial) {
    //    ; // wait for serial port to connect. Needed for native USB port only
    //}
    
}
// 2) Platform Start Function
void platformStart()
{
    
}
// 3) Platform Loop Delay Function
// Teensy 4.1 has 4 byte FIFOs for UARTs
// Teensy 4.1 has 16 Word FIFOs for SPI/Ethernet/UDP
void platformLoopDelay()
{
    consoleCharInt = Serial.read();
    motor0CharInt = Serial1.read();
    motor1CharInt = Serial2.read();
    motor2CharInt = Serial3.read();
    motor3CharInt = Serial4.read();    
}

#ifdef __USINGCONSOLEMENU

// 8) Platform API Functions (From Template?)
PlatformAPIFuncsTemplate(size + 1);

#endif
// 9) Global Execution System Instance
//executionSystemClass PLATFORM_EXESYS_NAME(PLATFORM_NAME)(uSEC_PER_CLOCK);
// 10) ExeSys API Functions (From Template?)
ExeSysAPIFuncsTemplate