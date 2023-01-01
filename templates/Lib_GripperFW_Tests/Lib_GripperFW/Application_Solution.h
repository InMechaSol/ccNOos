/** \file Application_Solution.h
*   \brief <a href="https://www.inmechasol.org/" target="_blank">IMS</a>:
		<a href="https://github.com/InMechaSol/ccNOos" target="_blank">ccNOos</a>,
		Declarations for straight C and C++ 

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
#ifndef __CCGRIPPER__
#define __CCGRIPPER__  

// Base ccNOos includes
#include "execution_system.h"
#include "console_menu.h"
#include "packets_api.h"

// Gripper and Device includes
#include "SmartMotorsDEV.h"
#include "SuctionExtentionDEV.h"

// The main Gripper module
#define Mn ccGripper

////////////////////////////////////////////////////////////////////////////
// Version Strings for Compile time and Run time
//
#define ccGripper_MajorVer 0
#define ccGripper_MinorVer 1
#define ccGripper_BuildNumber 1267 

#define ccGripper_DevString dev
#define ccGripper_VerDate 24NOV2022

#ifdef ccGripper_ReleaseBuild
#define ccGripper_VersionNumber ccGripper_MajorVer.ccGripper_MinorVer.ccGripper_BuildNumber
#endif
#ifndef ccGripper_ReleaseBuild
#define ccGripper_VersionNumber ccGripper_MajorVer.ccGripper_MinorVer.ccGripper_BuildNumber-ccGripper_DevString
#endif

const char* ccGripper_VerString();
const char* ccGripper_VerDateString();

#define ccGripperVersionsTemplate \
    const char* ccGripper_VerString()\
    {\
        return xstr(ccGripper_VersionNumber);\
    }\
    const char* ccGripper_VerDateString()\
    {\
        return xstr(ccGripper_VerDate);\
    }


//
// END - Version Strings for Compile time and Run time
////////////////////////////////////////////////////////////////////////////

#define EVENODDMOTORS (1)
#define NUMMOTORS (4)
#define iCURRENT (0)
#define iLAST (1)
#define OPSLOGSIZE (20)
enum enumGripperMode
{
    gripper_uninitialized,
    gripper_opened,
    gripper_closed,
    gripper_stalled,
    gripper_inmotion
};

// Main SatComACS Data Structure
MODdeclareSTRUCT(Mn)
{
    COMPMODFIRST;
    // coordinated gripper control Module
    enum enumGripperMode GripperMode[2];
    UI_8 execAPIsMainLoop, execDEVsMainLoop;
    int OperationsLog[OPSLOGSIZE];
    int OpsLogIndex;
    int OpsLogSize, OpsLogSizeLast;
    UI_8 AutoUpdate;
    UI_32 AutoUpdateNow, AutoUptateThen;
    UI_32 AutoUpdateOnAt;
    UI_32 TimeStamp;
    UI_8 TriggerOpen, TriggerClose, TriggerLatch, TriggerStop;
    UI_8 PackageCMDOpen, PackageCMDClose, PackageCMDLatch, PackageCMDStop;

    // gripper Device Modules
    struct smartMotorStruct SmartMotors[NUMMOTORS];    
    struct vacuumExtendStruct VacuumExtend;
    
    // gripper API Device Modules
    struct packAPIStruct PacketsAPI;
    struct uiStruct ConsoleMenu;
};

#define MODdeclareCREATEINS 
#define MODcallCREATEINS 

MODdeclareCREATE(Mn)(MODdeclareCREATEINS);

// Re-usable, portable, cross-platform (ccNOosTests setup() function)
MODdeclareSETUP(Mn);
// Re-usable, portable, cross-platform (ccNOosTests loop() function)
MODdeclareLOOP(Mn);
// Re-usable, portable, cross-platform (ccNOosTests systick() function)
MODdeclareSYSTICK(Mn);

#ifdef __USINGCONSOLEMENU 

enum currentMenuAction
{
    cA_NoAction,
    cA_Enter,
    cA_Up,
    cA_Down,
    cA_Forward,
    cA_Back,
    cA_Status,
    cA_Acquire
};

enum currentMenuNode
{
    cM_RootNode,
    cM_MainMenu = 0,
    cM_Devices,
    cM_ExecutionSystem,
    cM_devMOT0,
    cM_devMOT1,
    cM_devMOT2,
    cM_devMOT3
};

enum uiAccessLevel
{
    uiLevel_Observer = 0,
    uiLevel_Operator,
    uiLevel_Administrator,
    uiLevel_Developer
};

//// Module Console Menu Print and Parse
MODdeclarePRINTm(Mn);
MODdeclarePARSEi(Mn);


void linkAPIioDevices(struct ccGripperStruct* gripperStructPtrIn);

// Parsing - Common - Console Menu
void parseUserLevel(struct uiStruct* uiStructPtrIn);
void goParsedAction(enum currentMenuAction theCurrentAction, struct ccGripperStruct* gripperStructPtrIn, struct uiStruct* uiStructPtrIn);
void parseConsoleKeyPadAPI(struct ccGripperStruct* gripperStructPtrIn, struct uiStruct* uiStructPtrIn);

// Parsing/Printing - ccGripper - Console Menu
void parseGripperFWConsoleAPI(struct ccGripperStruct* gripperStructPtrIn, struct uiStruct* uiStructPtrIn);
void printGripperFWConsoleAPI(struct ccGripperStruct* gripperStructPtrIn, struct uiStruct* uiStructPtrIn);


// Parsing/Packaging - Common - Packets API


// Parsing/Packaging - ccGripper - Packets API
void parseGripperPacketsAPI(struct ccGripperStruct* gripperStructPtrIn, struct packAPIStruct* packStructPtrIn);
void packageGripperPacketsAPI(struct ccGripperStruct* gripperStructPtrIn, struct packAPIStruct* packStructPtrIn);

#else
#error Console Required for GripperFW Module
#endif


////////////////////////////////////////////////////////////////////////////////
// C ccNOosTests Example Application - built from computeModuleClass and Execution System
#define __PLATFORM_APP_CTEMPLATE(MODNAME) \
    struct linkedEntryPointStruct setupListHead = {\
        nullptr,\
        (struct computeModuleStruct*)&MODdataINST(Mn),\
        MODsetup(Mn)\
        };\
    struct linkedEntryPointStruct loopListHead = {\
        nullptr,\
        (struct computeModuleStruct*)&MODdataINST(Mn),\
        MODloop(Mn)\
        };\
    struct executionEntryStruct exeEntryPoints = {\
        &setupListHead,\
        &loopListHead,\
        nullptr,\
        &setupListHead\
        };\
    void applicationConfig()\
    {\
        exeSystem = CreateExecutionSystemStruct(uSEC_PER_CLOCK);\
        MODdataINST(Mn) = MODstructCREATE(Mn)();\
    }
#define  PLATFORM_APP_CTEMPLATE(MODNAME) __PLATFORM_APP_CTEMPLATE(MODNAME)

#ifdef __cplusplus
////////////////////////////////////////////////////////////////////////////////
// C++ ccNOosTests Example Class - built from computeModuleClass
class MODCLASS_NAME(Mn) : public computeModuleClass {
protected:
    MODdeclareDATA(Mn);
public:
    MODCLASS_NAME(Mn)(MODdeclareCREATEINS);
    MODCLASS_SETUP_INLINE(Mn);
    MODCLASS_LOOP_INLINE(Mn);
    MODCLASS_SYSTICK_INLINE(Mn);
    MODCLASS_ExcpHndlr_INLINE(Mn);
};

////////////////////////////////////////////////////////////////////////////////
// C++ ccNOosTests Example Application - built from computeModuleClass and Execution System
#define __PLATFORM_APP_CLASS(MODNAME) class theApplicationClass{\
    public:\
    linkedEntryPointClass setupListHead;\
    linkedEntryPointClass loopListHead;\
    linkedEntryPointClass systickListHead;\
    linkedEntryPointClass exceptionListHead;\
    MODCLASS_NAME(MODNAME) MODNAME##CompMod;\
    executionSystemClass* theExecutionSystemPtr;\
    theApplicationClass() :\
        MODNAME##CompMod(),\
        setupListHead(& MODNAME##CompMod, nullptr),\
        loopListHead(& MODNAME##CompMod, nullptr),\
        systickListHead(nullptr, nullptr),\
        exceptionListHead(&MODNAME##CompMod, nullptr)\
    {\
        theExecutionSystemPtr = &exeSystem;\
        theExecutionSystemPtr->LinkTheListsHead(\
            &setupListHead,\
            &loopListHead,\
            &systickListHead,\
            &exceptionListHead\
        );\
    }\
}
#define PLATFORM_APP_CLASS(MODNAME) __PLATFORM_APP_CLASS(MODNAME)


#endif // !__cplusplus
#endif // !__CCGRIPPER__
