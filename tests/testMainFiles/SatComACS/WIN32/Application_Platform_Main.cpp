//////////////////////////////////
// Include Application and cNOos
#include "PlatformApp_Serialization.h"

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
#ifndef PLATFORM_APP_CLASS
#error PLATFORM_APP_CLASS macro must be defined in application code
#endif

////////////////////////////////
// Compile Error if Examples/Tests not defining
#if PLATFORM_NAME==Win32
#include "Platform_Win32.h"
#else
#error "No Platform Selected for Compile!"
#endif

///////////////////////////////////////////////////////////////////////
// Module Data Structure or Module Data Class 
//   -Declared from Macro Template
//   -Macro Template is Defined in the Application Solution Header
///////////////////////////////////////////////////////////////////////
PLATFORM_APP_CLASS(Mn);

///////////////////////////////////////////////////////////////////////
// Platform and Application Specific IO Device Functions
#include "tcp_comms.h"
#include "serial_comms.h"  
#include "adafruit_ft232h.h"

// APT Module Devices
struct portParametersStruct GPSPortParams;
struct portParametersStruct eCompPortParams;
nbserial_class GPS_NBSerial(&GPSPortParams);
nbserial_class eComp_NBSerial(&eCompPortParams);

// Log and Config API Devices
struct logStruct StdLogStruct;
struct configStruct StdConfStruct;

// Menu API Devices
struct portParametersStruct LCDKeyPadPortParams;
nbserial_class LCDKeyPad_NBSerial(&LCDKeyPadPortParams);

struct tcpServerStruct ConsoleMenuTCPServerStruct;
struct tcpStruct ConsoleMenuTCPStructTemp;
tcp_server_class ConsoleMenu_tcpServer(&ConsoleMenuTCPServerStruct);


void linkAPIioDevices(struct SatComACSStruct* satcomacsStructPtrIn)
{
    // APT Module IO Devices are linked at the ReadGPS and ReadeComp Functions
    GPSPortParams = buildportParametersStruct("\\\\.\\COM31", 9600);
    openComPort(&GPSPortParams);
    eCompPortParams = buildportParametersStruct("\\\\.\\COM30", 19200);
    openComPort(&eCompPortParams);

    // Menu API Devices are linked here
    LCDKeyPadPortParams = buildportParametersStruct("\\\\.\\COM9", 9600);
    openComPort(&LCDKeyPadPortParams);
    satcomacsStructPtrIn->LCDKeyPad.devptr = &LCDKeyPadPortParams.serialdev.devdata;
    satcomacsStructPtrIn->LCDKeyPad.devptr->triggerWriteOperation = ui8TRUE;

    
    ConsoleMenuTCPServerStruct = createTCPServerStruct();
    ConsoleMenuTCPStructTemp = createTCPStruct();
    ConsoleMenuTCPServerStruct.listenPort = 55500;
    OpenTCPServer(&ConsoleMenuTCPServerStruct);
    satcomacsStructPtrIn->ConsoleMenu.devptr = &ConsoleMenuTCPServerStruct.tcpData.devdata;
    satcomacsStructPtrIn->ConsoleMenu.showHelp = ui8TRUE;
    satcomacsStructPtrIn->ConsoleMenu.devptr->triggerWriteOperation = ui8TRUE;

    // Config and Log API Devices are linked here
    ; // currently linked to std I/O via writeLogLine and readConfigLine Functions


}
std::thread stdInThread;
UI_8 stdInThreadRunning = ui8FALSE;
bool runONCE = true; // to launch std::in thread once
void readStdIn(char* inStringPtr)
{
    do {
        if (stdInThreadRunning == ui8TRUE)
        {
            std::cin >> inStringPtr;
            stdInThreadRunning = ui8FALSE;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (true);
}
// 4) Basic ability for user console input via any io device
static enum tcpServerClientStatus lastTCPConsoleState = tcpstat_uninitialized;
void GetMenuChars(struct uiStruct* uiStructPtrin)
{
    // if Consolue Menu
    if (uiStructPtrin->devptr == &ConsoleMenuTCPServerStruct.tcpData.devdata)
    {
        // non-blocking server mainatainance
        ConsoleMenu_tcpServer.ShutdownRestartServerSocket();

        // non-blocking check for client connection
        ConsoleMenu_tcpServer.ServerListenAccept();

        // on each client connection, trigger menu write and show help
        if (lastTCPConsoleState != ConsoleMenu_tcpServer.getTCPStatus())
        {
            lastTCPConsoleState = ConsoleMenu_tcpServer.getTCPStatus();
            if (lastTCPConsoleState==tcpstat_connected)
            {
                uiStructPtrin->showHelp = ui8TRUE;
                uiStructPtrin->devptr->triggerWriteOperation = ui8TRUE;
            }
        }

        // non-blocking check for client packet
        if (ConsoleMenu_tcpServer.ReadDev() > 0)
        {
            uiStructPtrin->devptr->newDataReadIn = ui8TRUE;
            uiStructPtrin->parseIndex = 0;
        }
    }
    // if LCD KeyPad
    else if (uiStructPtrin->devptr == &LCDKeyPadPortParams.serialdev.devdata)
    {
        // check for chars from serial port
        ;
    }
}
// 5) Basic ability for user console output
void WriteMenuLine(struct uiStruct* uiStructPtrin)
{
    // if Consolue Menu
    if (uiStructPtrin->devptr == &ConsoleMenuTCPServerStruct.tcpData.devdata)
    {
        // based on state
        // write to tcp socket

        if (uiStructPtrin->clearScreen) {
            ConsoleMenuTCPStructTemp.devdata.numbytes2Write = ConsoleMenu_tcpServer.GetDevPtr()->numbytes2Write;  
            stringInit(ConsoleMenuTCPStructTemp.devdata.outbuff.charbuff, ConsoleMenu_tcpServer.GetDevPtr()->outbuff.charbuff); 
            stringInit(ConsoleMenu_tcpServer.GetDevPtr()->outbuff.charbuff, terminalClearString());
            ConsoleMenu_tcpServer.GetDevPtr()->numbytes2Write = stringLength(terminalClearString());
            TCPWrite(&ConsoleMenuTCPServerStruct.tcpData);
            stringInit(ConsoleMenu_tcpServer.GetDevPtr()->outbuff.charbuff, ConsoleMenuTCPStructTemp.devdata.outbuff.charbuff);
            ConsoleMenu_tcpServer.GetDevPtr()->numbytes2Write = ConsoleMenuTCPStructTemp.devdata.numbytes2Write;
            uiStructPtrin->clearScreen = ui8FALSE;
        }
        if(ConsoleMenu_tcpServer.WriteDev()>0);
        if (uiStructPtrin->showPrompt) {
            ConsoleMenuTCPStructTemp.devdata.numbytes2Write = ConsoleMenu_tcpServer.GetDevPtr()->numbytes2Write;
            stringInit(ConsoleMenuTCPStructTemp.devdata.outbuff.charbuff, ConsoleMenu_tcpServer.GetDevPtr()->outbuff.charbuff);
            stringInit(ConsoleMenu_tcpServer.GetDevPtr()->outbuff.charbuff, terminalPromptString(uiStructPtrin->currentUserLevel));
            ConsoleMenu_tcpServer.GetDevPtr()->numbytes2Write = stringLength(terminalPromptString(uiStructPtrin->currentUserLevel));
            TCPWrite(&ConsoleMenuTCPServerStruct.tcpData);
            stringInit(ConsoleMenu_tcpServer.GetDevPtr()->outbuff.charbuff, ConsoleMenuTCPStructTemp.devdata.outbuff.charbuff);
            ConsoleMenu_tcpServer.GetDevPtr()->numbytes2Write = ConsoleMenuTCPStructTemp.devdata.numbytes2Write;
            uiStructPtrin->showPrompt = ui8FALSE;
        }
    }
    // if LCD KeyPad
    else if (uiStructPtrin->devptr == &LCDKeyPadPortParams.serialdev.devdata)
    {
        ;
    }

}
// 6) (Optional) Logging Output
void WriteLogLine(struct logStruct* logStructPtrin)
{
    // Send log to stdout
    std::cout << &logStructPtrin->devptr->outbuff.charbuff[0];
}
// 7) (Optional) Config Input
void ReadConfigLine(struct configStruct* configStructPtrin)
{
    // Allow for configuration via stdin
    if (stdInThreadRunning == ui8FALSE)
    {
        if (runONCE)
        {
            stdInThread = std::thread(readStdIn, &configStructPtrin->devptr->inbuff.charbuff[0]);
            runONCE = false;
            stdInThreadRunning = ui8TRUE;
        }
        else if (configStructPtrin->devptr->triggerWriteOperation == ui8FALSE)
        {
            configStructPtrin->devptr->newDataReadIn = ui8TRUE;
            //configStructPtrin->parseIndex = 0;
            stdInThreadRunning = ui8TRUE;
        }
    }
}


UI_8 readGPS(struct gpsStruct* gpsStructPtrIn)
{
    GPSPortParams.serialdev.devdata.numbytes2Read = 1;
    GPSPortParams.serialdev.readIndex = 0;
    gpsStructPtrIn->devptr = &GPSPortParams.serialdev;
    int iNewLine = 0;
    int tries = 0;
    do {


        if (GPS_NBSerial.ReadDev() == GPSPortParams.serialdev.devdata.numbytes2Read)
        {
            if (GPSPortParams.serialdev.devdata.inbuff.charbuff[GPSPortParams.serialdev.readIndex] == '\n')
            {
                iNewLine = GPSPortParams.serialdev.readIndex;
            }
            GPSPortParams.serialdev.readIndex += GPSPortParams.serialdev.devdata.numbytes2Read;
        }
        else if (GPSPortParams.serialdev.readIndex > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            tries++;
        }
        else
            break;


    } while (GPSPortParams.serialdev.readIndex < charBuffMax && iNewLine == 0 && tries < 5);

    GPSPortParams.serialdev.devdata.inbuff.charbuff[iNewLine] = '\0';

    if (iNewLine > 6)
        return ui8TRUE;
    else
        return ui8FALSE;
}
UI_8 readEcompass(struct eCompStruct* eCompStructPtrIn)
{
    eCompPortParams.serialdev.devdata.numbytes2Read = 1;
    eCompPortParams.serialdev.readIndex = 0;
    eCompStructPtrIn->devptr = &eCompPortParams.serialdev;
    int iNewLine = 0;
    int tries = 0;
    do {


        if (eComp_NBSerial.ReadDev() == eCompPortParams.serialdev.devdata.numbytes2Read)
        {
            if (eCompPortParams.serialdev.devdata.inbuff.charbuff[eCompPortParams.serialdev.readIndex] == '\n')
            {
                iNewLine = eCompPortParams.serialdev.readIndex;
            }
            eCompPortParams.serialdev.readIndex += eCompPortParams.serialdev.devdata.numbytes2Read;
        }
        else if (eCompPortParams.serialdev.readIndex > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            tries++;
        }
        else
            break;


    } while (eCompPortParams.serialdev.readIndex < charBuffMax && iNewLine == 0 && tries < 5);

    eCompPortParams.serialdev.devdata.inbuff.charbuff[iNewLine] = '\0';

    if (iNewLine > 10)
        return ui8TRUE;
    else
        return ui8FALSE;
}


#define SETPIN_TX_C16(high_low)    
#define SETPIN_TX_C8(high_low)     
#define SETPIN_TX_C4(high_low)     
#define SETPIN_TX_C2(high_low)     
#define SETPIN_TX_C1(high_low)     
#define SETPIN_TX_C0_25(high_low)  
#define SETPIN_TX_C0_50(high_low)  
#define SETPIN_TX_LE(high_low)     

#define SETPIN_RX_C16(high_low)    
#define SETPIN_RX_C8(high_low)     
#define SETPIN_RX_C4(high_low)     
#define SETPIN_RX_C2(high_low)     
#define SETPIN_RX_C1(high_low)     
#define SETPIN_RX_C0_25(high_low)  
#define SETPIN_RX_C0_50(high_low)  
#define SETPIN_RX_LE(high_low)     

#define SETPIN_XX_C16(high_low)    
#define SETPIN_XX_C8(high_low)     
#define SETPIN_XX_C4(high_low)     
#define SETPIN_XX_C2(high_low)     
#define SETPIN_XX_C1(high_low)     
#define SETPIN_XX_C0_25(high_low)  
#define SETPIN_XX_C0_50(high_low)  
#define SETPIN_XX_LE(high_low)     

#define SETPIN_MDM1_SW(high_low)   
#define SETPIN_MDM2_SW(high_low)   
#define SETPIN_MDM3_SW(high_low)   
#define SETPIN_MDM4_SW(high_low)   

SatComACS_TxRx_WriteFuncsTemplate(std::this_thread::sleep_for(std::chrono::milliseconds(1)))


void readFreqConv(struct freqConvStruct* freqConvStructPtrIn) { ; }
void writeFreqConv(struct freqConvStruct* freqConvStructPtrIn) { ; }
void readPowerMeter(struct powerMeterStruct* powerMeterStructPtrIn) { ; }
void writePowerMeter(struct powerMeterStruct* powerMeterStructPtrIn) { ; }


///////////////////////////////////////////////////////////////////////
// Application Data Instances are Created here (Platform Specific)
theApplicationClass theApplicationExample;


////////////////////////////////////////////////////////////////////////////////
// Platform Main Entry Points call ExeSys Area Functions
CPP_OS_MAIN_TEMPLATE