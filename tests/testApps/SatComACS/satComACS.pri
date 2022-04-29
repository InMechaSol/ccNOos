## SatComACS depends on following directories
if(!defined(SatComACSappDIR,var)) {
    error("SatComACSappDIR is not defined")
}
## SatComACS depends on the ccNOos project
## if the ccNOos project hasn't already been loaded elsewhere
## it should be loaded here, now
if(!defined(ccNOos_CPPBuild,test)) {
    if(!defined(ccNOosDIR,var)) {
        error("ccNOosDIR is not defined")
    }
    include($$ccNOosDIR/ccNOos.pri)
}


INCLUDEPATH += $$SatComACSappDIR

INCLUDEPATH += $$SatComACSappDIR/apiModules/ConsoleMenu
INCLUDEPATH += $$SatComACSappDIR/apiModules/LCDKeyPad
INCLUDEPATH += $$SatComACSappDIR/deviceModules/APT_WMM
INCLUDEPATH += $$SatComACSappDIR/deviceModules/TPM
INCLUDEPATH += $$SatComACSappDIR/deviceModules/TxRx

HEADERS += $$SatComACSappDIR/Application_Solution.h
HEADERS += $$SatComACSappDIR/PlatformApp_Serialization.h
HEADERS += $$SatComACSappDIR/apiModules/ConsoleMenu/MenuAPI.h
HEADERS += $$SatComACSappDIR/apiModules/LCDKeyPad/LCDapi.h
HEADERS += $$SatComACSappDIR/deviceModules/APT_WMM/APTmodule.h
HEADERS += $$SatComACSappDIR/deviceModules/TPM/TPMmodule.h
HEADERS += $$SatComACSappDIR/deviceModules/TxRx/TxRxmodule.h

if(ccNOos_CPPBuild()) { # a cpp build uses .c as header

    HEADERS += $$SatComACSappDIR/Application_Solution.c
    SOURCES += $$SatComACSappDIR/Application_Solution.cpp                 # only in cpp build
    HEADERS += $$SatComACSappDIR/PlatformApp_Serialization.c
    SOURCES += $$SatComACSappDIR/PlatformApp_Serialization.cpp            # only in cpp build
    HEADERS += $$SatComACSappDIR/apiModules/ConsoleMenu/MenuAPI.c
    SOURCES += $$SatComACSappDIR/apiModules/ConsoleMenu/MenuAPI.cpp       # only in cpp build
    HEADERS += $$SatComACSappDIR/apiModules/LCDKeyPad/LCDapi.c
    SOURCES += $$SatComACSappDIR/apiModules/LCDKeyPad/LCDapi.cpp          # only in cpp build
    HEADERS += $$SatComACSappDIR/deviceModules/APT_WMM/APTmodule.c
    SOURCES += $$SatComACSappDIR/deviceModules/APT_WMM/APTmodule.cpp      # only in cpp build
    HEADERS += $$SatComACSappDIR/deviceModules/TPM/TPMmodule.c
    SOURCES += $$SatComACSappDIR/deviceModules/TPM/TPMmodule.cpp          # only in cpp build
    HEADERS += $$SatComACSappDIR/deviceModules/TxRx/TxRxmodule.c
    SOURCES += $$SatComACSappDIR/deviceModules/TxRx/TxRxmodule.cpp        # only in cpp build

} else { # a straight c build uses .c as source

    SOURCES += $$SatComACSappDIR/Application_Solution.c
    SOURCES += $$SatComACSappDIR/PlatformApp_Serialization.c
    SOURCES += $$SatComACSappDIR/apiModules/ConsoleMenu/MenuAPI.c
    SOURCES += $$SatComACSappDIR/apiModules/LCDKeyPad/LCDapi.c
    SOURCES += $$SatComACSappDIR/deviceModules/APT_WMM/APTmodule.c
    SOURCES += $$SatComACSappDIR/deviceModules/TPM/TPMmodule.c
    SOURCES += $$SatComACSappDIR/deviceModules/TxRx/TxRxmodule.c

}
