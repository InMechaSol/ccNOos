#include "../executionSystem/execution_system.h"    
#include "../consoleMenu/console_menu.h"   


///////////////////////////////////////////////////////////////////////
// SatCom Tunable Power Meter Example
///////////////////////////////////////////////////////////////////////
#ifndef COMPILE_TESTS
#ifdef EXAMPLE_POWER_METER

#ifdef Mn
#error ccNOos_Tests: Multiple Examples Selected for Compilation, Not Permitted
#else
#define Mn PowerMeter
#endif

#define SPIBYTESMAX (80u)
#define charBuffMax (80u)

MODdeclareSTRUCT(Mn)
{
    COMPMODFIRST;
    UI_8 ADC_ChipSelect;
    UI_8 DownConverter_ChipSelect;
    UI_8 BytesRead, Bytes2Write;
    UI_8 SPIBytesOut[SPIBYTESMAX];
    UI_8 SPIBytesIn[SPIBYTESMAX];
    UI_8 TriggerUpdate;
    UI_32 DownLinkIF_MHZ;
#ifdef __USINGCONSOLEMENU
    int printMReturn, parseIReturn;
    UI_8    charsRead, chars2Write;
    char    charbuff_In[charBuffMax];
    char    charbuff_Out[charBuffMax];
#endif
    
};

#define MODdeclareCREATEINS 
#define MODcallCREATEINS 

MODdeclareCREATE(Mn)(MODdeclareCREATEINS);

// platform and application specific io device functions
void WriteSPIDevice(MODdeclarePTRIN(Mn));
void ReadSPIDevice(MODdeclarePTRIN(Mn));
void WriteChipSelect(MODdeclarePTRIN(Mn));

// Re-usable, portable, cross-platform (power meter example setup() function)
MODdeclareSETUP(Mn);
// Re-usable, portable, cross-platform (power meter example  loop() function)
MODdeclareLOOP(Mn);
// Re-usable, portable, cross-platform (power meter example  systick() function)
MODdeclareSYSTICK(Mn);
// Re-usable, portable, cross-platform (power meter example  print/parse functions)
#ifdef __USINGCONSOLEMENU 
MODdeclarePRINTm(Mn);
MODdeclarePARSEi(Mn);
#endif
////////////////////////////////////////////////////////////////////////////////
// C SysTickClock Example Application - built from computeModuleClass and Execution System
#define __PLATFORM_APP_CTEMPLATE(PLATNAME,MODNAME) \
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
        PLATFORM_EXESYS_NAME(PLATFORM_NAME) = CreateExecutionSystemStruct(\
                uSEC_PER_CLOCK);\
        MODdataINST(Mn) = MODstructCREATE(Mn)();\
    }
#define  PLATFORM_APP_CTEMPLATE(PLATNAME,MODNAME) __PLATFORM_APP_CTEMPLATE(PLATNAME,MODNAME)

#ifdef __cplusplus
////////////////////////////////////////////////////////////////////////////////
// C++ SysTickClock Example Class - built from computeModuleClass
class MODCLASS_NAME(Mn) : public computeModuleClass {
private:
    MODdeclareDATA(Mn);
public:
    MODCLASS_NAME(Mn)(MODdeclareCREATEINS);
    MODCLASS_SETUP_INLINE(Mn);
    MODCLASS_LOOP_INLINE(Mn);
    MODCLASS_SYSTICK_INLINE(Mn);
    MODCLASS_ExcpHndlr_INLINE(Mn);
};

////////////////////////////////////////////////////////////////////////////////
// C++ SysTickClock Example Application - built from computeModuleClass and Execution System
#define __PLATFORM_APP_CLASS(PLATNAME,MODNAME) class PLATFORM_APP_NAME(PLATNAME){\
    public:\
    linkedEntryPointClass setupListHead;\
    linkedEntryPointClass loopListHead;\
    linkedEntryPointClass systickListHead;\
    linkedEntryPointClass exceptionListHead;\
    MODCLASS_NAME(MODNAME) MODNAME##CompMod;\
    executionSystemClass* MODNAME##ExecutionSystemPtr;\
    PLATFORM_APP_NAME(PLATNAME)() :\
        MODNAME##CompMod(),\
        setupListHead(& MODNAME##CompMod, nullptr),\
        loopListHead(& MODNAME##CompMod, nullptr),\
        systickListHead(nullptr, nullptr),\
        exceptionListHead(&MODNAME##CompMod, nullptr)\
    {\
        MODNAME##ExecutionSystemPtr = & PLATFORM_EXESYS_NAME(PLATNAME);\
        MODNAME##ExecutionSystemPtr->LinkTheListsHead(\
            &setupListHead,\
            &loopListHead,\
            &systickListHead,\
            &exceptionListHead\
        );\
    }\
}
#define PLATFORM_APP_CLASS(PLATNAME,MODNAME) __PLATFORM_APP_CLASS(PLATNAME,MODNAME)


#endif // !__cplusplus
#endif // !EXAMPLE_POWER_METER
#endif // COMPILE_TESTS
