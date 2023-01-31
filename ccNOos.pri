## Indicate in messages if building for C or C++ project
## (C++11 explicit check, otherwise C)
if(CONFIG(c++11)) { message(ccNOos for C++) }
 else { message(ccNOos for C) }

## ccNOos depends on following directories
if(!defined(ccNOosDIR,var)) {
    error("ccNOosDIR is not defined")
} else {
message(ccNOos Directory is at $$ccNOosDIR)
}

# define a Test function to determine if ccNOos is building for c or c++
defineTest(ccNOos_CPPBuild) {
    if( CONFIG(c++11) ) {
        return(true)
    } else {
        return(false)
    }
}

# use Test function to set extensions for object generation
if(ccNOos_CPPBuild()) { # a cpp build uses .c as header
    QMAKE_EXT_CPP = .cpp
    QMAKE_EXT_H = .h .hpp .c
} else { # a straight c build uses .c as source
    QMAKE_EXT_H = .h
}

## ccNOos Core Must be Included
iPath = $$ccNOosDIR/executionSystem
fName = $$iPath/execution_system
INCLUDEPATH += $$iPath
HEADERS += $${fName}.h
HEADERS += $$iPath/version_config.h
if(!ccNOos_OnlyHeaders){
    if(ccNOos_CPPBuild()) { # a cpp build uses .c as header
        HEADERS += $${fName}.c
        SOURCES += $${fName}_class.cpp # only in cpp build
    } else { # a straight c build uses .c as source
        SOURCES += $${fName}.c
    }
}
iPath = $$ccNOosDIR/computeModule
fName = $$iPath/compute_module
INCLUDEPATH += $$iPath
HEADERS += $${fName}.h
if(!ccNOos_OnlyHeaders){
    if(ccNOos_CPPBuild()) { # a cpp build uses .c as header
        HEADERS += $${fName}.c
        SOURCES += $${fName}_class.cpp # only in cpp build
    } else { # a straight c build uses .c as source
        SOURCES += $${fName}.c
    }
}
iPath = $$ccNOosDIR/consoleMenu
fName = $$iPath/console_menu
INCLUDEPATH += $$iPath
HEADERS += $${fName}.h
if(!ccNOos_OnlyHeaders){
    if(ccNOos_CPPBuild()) { # a cpp build uses .c as header
        HEADERS += $${fName}.c
        SOURCES += $${fName}_class.cpp # only in cpp build
    } else { # a straight c build uses .c as source
        SOURCES += $${fName}.c
    }
}
iPath = $$ccNOosDIR/packetsAPI
fName = $$iPath/packets_api
INCLUDEPATH += $$iPath
HEADERS += $${fName}.h
if(!ccNOos_OnlyHeaders){
    if(ccNOos_CPPBuild()) { # a cpp build uses .c as header
        HEADERS += $${fName}.c
        SOURCES += $${fName}_class.cpp # only in cpp build
    } else { # a straight c build uses .c as source
        SOURCES += $${fName}.c
    }
}
iPath = $$ccNOosDIR/ioDevice
fName = $$iPath/io_device
INCLUDEPATH += $$iPath
HEADERS += $${fName}.h
if(!ccNOos_OnlyHeaders){
    if(ccNOos_CPPBuild()) { # a cpp build uses .c as header
        HEADERS += $${fName}.c
        SOURCES += $${fName}_class.cpp # only in cpp build
    } else { # a straight c build uses .c as source
        SOURCES += $${fName}.c
    }
}


## ccLibs are Optional
if(ccNOosAllLibs|ccNOosLibs_acs) {
    iPath = $$ccNOosDIR/ccLibs/acs
    fName = $$iPath/satComControl
    INCLUDEPATH += $$iPath
    HEADERS += $${fName}.h
    if(!ccNOos_OnlyHeaders){
        if(ccNOos_CPPBuild()) { # a cpp build uses .c as header
            HEADERS += $${fName}.c
            SOURCES += $${fName}.cpp # only in cpp build
        } else { # a straight c build uses .c as source
            SOURCES += $${fName}.c
        }
    }
}

if(ccNOosAllLibs|ccNOosLibs_mcs) {
    DEFINES += __USE_MCS
    iPath = $$ccNOosDIR/ccLibs
    fName = $$iPath/abstractMATH
    INCLUDEPATH += $$iPath
    HEADERS += $${fName}.h
    iPath = $$ccNOosDIR/ccLibs/mcs
    fName = $$iPath/motionControl
    INCLUDEPATH += $$iPath
    HEADERS += $${fName}.h
    if(!ccNOos_OnlyHeaders){
        if(ccNOos_CPPBuild()) { # a cpp build uses .c as header
            HEADERS += $${fName}.c
            SOURCES += $${fName}.cpp # only in cpp build
        } else { # a straight c build uses .c as source
            SOURCES += $${fName}.c
        }
    }
    fName = $$iPath/motionControlSerialize
    INCLUDEPATH += $$iPath
    HEADERS += $${fName}.h
    if(!ccNOos_OnlyHeaders){
        if(ccNOos_CPPBuild()) { # a cpp build uses .c as header
            HEADERS += $${fName}.c
            SOURCES += $${fName}.cpp # only in cpp build
        } else { # a straight c build uses .c as source
            SOURCES += $${fName}.c
        }
    }
}


## devices are Optional
if(ccNOosAllDevs|ccNOosDevs_ADRF6650) {
    iPath = $$ccNOosDIR/ccLibs/devices/ADRF6650
    fName = $$iPath/ADRF6650
    INCLUDEPATH += $$iPath
    HEADERS += $${fName}.h
    if(!ccNOos_OnlyHeaders){
        if(ccNOos_CPPBuild()) { # a cpp build uses .c as header
            HEADERS += $${fName}.c
            SOURCES += $${fName}.cpp # only in cpp build
        } else { # a straight c build uses .c as source
            SOURCES += $${fName}.c
        }
    }
    HEADERS += $$ccNOosDIR/ccLibs/devices/ADRF6650/ADRF6650_Map.h
}
if(ccNOosAllDevs|ccNOosDevs_LTC2360) {
    iPath = $$ccNOosDIR/ccLibs/devices/LTC2360
    fName = $$iPath/LTC2360
    INCLUDEPATH += $$iPath
    HEADERS += $${fName}.h
    if(!ccNOos_OnlyHeaders){
        if(ccNOos_CPPBuild()) { # a cpp build uses .c as header
            HEADERS += $${fName}.c
            SOURCES += $${fName}.cpp # only in cpp build
        } else { # a straight c build uses .c as source
            SOURCES += $${fName}.c
        }
    }
}
if(ccNOosAllDevs|ccNOosDevs_HMR3300) {
    iPath = $$ccNOosDIR/ccLibs/devices/HMR3300
    fName = $$iPath/HMR3300
    INCLUDEPATH += $$iPath
    HEADERS += $${fName}.h
    if(!ccNOos_OnlyHeaders){
        if(ccNOos_CPPBuild()) { # a cpp build uses .c as header
            HEADERS += $${fName}.c
            SOURCES += $${fName}.cpp # only in cpp build
        } else { # a straight c build uses .c as source
            SOURCES += $${fName}.c
        }
    }
}
if(ccNOosAllDevs|ccNOosDevs_NEO_LEA_M8T) {
    iPath = $$ccNOosDIR/ccLibs/devices/NEO_LEA_M8T
    fName = $$iPath/NEO_LEA_M8T
    INCLUDEPATH += $$iPath
    HEADERS += $${fName}.h
    if(!ccNOos_OnlyHeaders){
        if(ccNOos_CPPBuild()) { # a cpp build uses .c as header
            HEADERS += $${fName}.c
            SOURCES += $${fName}.cpp # only in cpp build
        } else { # a straight c build uses .c as source
            SOURCES += $${fName}.c
        }
    }
}
if(ccNOosAllDevs|ccNOosDevs_MCP23017) {
    iPath = $$ccNOosDIR/ccLibs/devices/MCP23017
    fName = $$iPath/MCP23017
    INCLUDEPATH += $$iPath
    HEADERS += $${fName}.h
    if(!ccNOos_OnlyHeaders){
        if(ccNOos_CPPBuild()) { # a cpp build uses .c as header
            HEADERS += $${fName}.c
            SOURCES += $${fName}.cpp # only in cpp build
        } else { # a straight c build uses .c as source
            SOURCES += $${fName}.c
        }
    }
}
if(ccNOosAllDevs|ccNOosDevs_MOP_VFD) {
    #ccNOosFileSetAdd($$ccNOosDIR/ccLibs/devices/MOP_VFD/ ){ message(done) }
}
if(ccNOosAllDevs|ccNOosDevs_HMC1119) {
    iPath = $$ccNOosDIR/ccLibs/devices/HMC1119
    fName = $$iPath/HMC1119
    INCLUDEPATH += $$iPath
    HEADERS += $${fName}.h
    if(!ccNOos_OnlyHeaders){
        if(ccNOos_CPPBuild()) { # a cpp build uses .c as header
            HEADERS += $${fName}.c
            SOURCES += $${fName}.cpp # only in cpp build
        } else { # a straight c build uses .c as source
            SOURCES += $${fName}.c
        }
    }
}
