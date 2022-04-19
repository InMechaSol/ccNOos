# Platform and Application Specific
# - The Full Solution Configuration to produce Compiled, Linked, Binary Output files
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += \
    PLATFORM_QTCreatorC \
    __USINGCONSOLEMENU \
    __USINGFLOATPRINTF

############################################################ LIBs
# Select All Libs
CONFIG += ccNOosAllLibs
# Select All Devices
CONFIG += ccNOosAllDevs

include($$ccNOosDIR/ccNOos.pri)

INCLUDEPATH += $$ccNOosTestsPlatformDIR
HEADERS += $$ccNOosTestsPlatformDIR/Platform_QTCreatorC.h

INCLUDEPATH += $$ccNOosTestsAppDIR
INCLUDEPATH += $$ccNOosTestsMainsDIR
HEADERS += $$ccNOosTestsAppDIR/Application_Solution.h
HEADERS += $$ccNOosTestsAppDIR/PlatformApp_Serialization.h
if(ccNOos_CPPBuild()) { # a cpp build uses .c as header
    HEADERS += $$ccNOosTestsMainsDIR/Application_Platform_Main.c
    SOURCES += $$ccNOosTestsMainsDIR/Application_Platform_Main.cpp # only in cpp build
    HEADERS += $$ccNOosTestsAppDIR/Application_Solution.c
    SOURCES += $$ccNOosTestsAppDIR/Application_Solution.cpp # only in cpp build
    HEADERS += $$ccNOosTestsAppDIR/PlatformApp_Serialization.c
    SOURCES += $$ccNOosTestsAppDIR/PlatformApp_Serialization.cpp # only in cpp build
} else { # a straight c build uses .c as source
    SOURCES += $$ccNOosTestsMainsDIR/Application_Platform_Main.c
    SOURCES += $$ccNOosTestsAppDIR/Application_Solution.c
    SOURCES += $$ccNOosTestsAppDIR/PlatformApp_Serialization.c
}
