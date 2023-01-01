## Lib_GripperFW depends on following directories
if(!defined(gcControlFWDIR,var)) {
    error("gcControlFWDIR is not defined")
}
## Lib_GripperFW depends on the ccNOos project
## if the ccNOos project hasn't already been loaded elsewhere
## it should be loaded here, now
if(!defined(ccNOos_CPPBuild,test)) {
    if(!defined(ccNOosDIR,var)) {
        error("ccNOosDIR is not defined")
    }
    include($$ccNOosDIR/ccNOos.pri)
}


INCLUDEPATH += $$gcControlFWDIR

INCLUDEPATH += $$gcControlFWDIR/apiModules
INCLUDEPATH += $$gcControlFWDIR/deviceModules

HEADERS += $$gcControlFWDIR/Application_Solution.h
HEADERS += $$gcControlFWDIR/Application_Serialization.h
HEADERS += $$gcControlFWDIR/apiModules/ConsoleMenuAPI.h
HEADERS += $$gcControlFWDIR/apiModules/PacketInterfaceAPI.h
HEADERS += $$gcControlFWDIR/deviceModules/SmartMotorsDEV.h
HEADERS += $$gcControlFWDIR/deviceModules/SuctionExtentionDEV.h

if(ccNOos_CPPBuild()) { # a cpp build uses .c as header

    HEADERS += $$gcControlFWDIR/Application_Solution.c
    SOURCES += $$gcControlFWDIR/Application_Solution.cpp                    # only in cpp build
    HEADERS += $$gcControlFWDIR/Application_Serialization.c
    SOURCES += $$gcControlFWDIR/Application_Serialization.cpp               # only in cpp build
    HEADERS += $$gcControlFWDIR/apiModules/ConsoleMenuAPI.c
    SOURCES += $$gcControlFWDIR/apiModules/ConsoleMenuAPI.cpp               # only in cpp build
    HEADERS += $$gcControlFWDIR/apiModules/PacketInterfaceAPI.c
    SOURCES += $$gcControlFWDIR/apiModules/PacketInterfaceAPI.cpp           # only in cpp build
    HEADERS += $$gcControlFWDIR/deviceModules/SmartMotorsDEV.c
    SOURCES += $$gcControlFWDIR/deviceModules/SmartMotorsDEV.cpp            # only in cpp build
    HEADERS += $$gcControlFWDIR/deviceModules/SuctionExtentionDEV.c
    SOURCES += $$gcControlFWDIR/deviceModules/SuctionExtentionDEV.cpp       # only in cpp build

} else { # a straight c build uses .c as source

    SOURCES += $$gcControlFWDIR/Application_Solution.c
    SOURCES += $$gcControlFWDIR/Application_Serialization.c
    SOURCES += $$gcControlFWDIR/apiModules/ConsoleMenuAPI.c
    SOURCES += $$gcControlFWDIR/apiModules/PacketInterfaceAPI.c    
    SOURCES += $$gcControlFWDIR/deviceModules/SmartMotorsDEV.c
    SOURCES += $$gcControlFWDIR/deviceModules/SuctionExtentionDEV.c

}
