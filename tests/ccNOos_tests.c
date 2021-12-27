/** \file ccNOos_tests.c
*   \brief Cross-Platform Portable ccNOos Tests Definitions

   Copyright 2021 InMechaSol, Inc

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

Notes:

This source demonstrates the usage of the ccNOos library and its dependence
on other files in the library.  A platform specific main(.c,.cpp) file need
only instantiate, configure, then execute the execution system.  

*/
#include "ccNOos_tests.h"

#ifdef COMPILE_TESTS
// Re-usable, portable, cross-platform (ccNOosTests create() function)
MODdeclareCREATE(Mn)(MODdeclareCREATEINS)
{
    MODdeclareSTRUCT(Mn) outStruct;
    outStruct.compMod = CreateComputeModuleStruct();
        
    outStruct.float_0 = 0.0;
    outStruct.float_1 = outStruct.float_0;
    outStruct.double_0 = 0.0;
    outStruct.double_1 = outStruct.double_0;
    
    outStruct.ui64_0 = 0.0;
    outStruct.ui64_1 = outStruct.ui64_0;
    outStruct.i64_0 = 0.0;
    outStruct.i64_1 = outStruct.i64_0;
    
    outStruct.ui32_0 = 0.0;
    outStruct.ui32_1 = outStruct.ui32_0;
    outStruct.i32_0 = 0.0;
    outStruct.i32_1 = outStruct.i32_0;
    
    outStruct.ui16_0 = 0.0;
    outStruct.ui16_1 = outStruct.ui16_0;
    outStruct.i16_0 = 0.0;
    outStruct.i16_1 = outStruct.i16_0;
    
    outStruct.ui8_0 = 0.0;
    outStruct.ui8_1 = outStruct.ui8_0;
    outStruct.i8_0 = 0.0;
    outStruct.i8_1 = outStruct.i8_0;
    
    outStruct.charsRead = 1u;
    outStruct.chars2Write = 0u;

#ifdef __USINGCONSOLEMENU
    UI_8 i = 0;
    for (i = 0; i < charBuffMax; i++)
        outStruct.charbuff_In[i] = 0x00;
    for (i = 0; i < charBuffMax; i++)
        outStruct.charbuff_Out[i] = 0x00;
        
#endif

    outStruct.SerializationTestReturn = RETURN_TEST_PASSED;
    outStruct.TestState = 0x0000;
        
    return outStruct;
}


// Re-usable, portable, cross-platform (ccNOosTests setup() function)
MODdeclareSETUP(Mn)
{
    MODDATAPTR_ERROR_RETURN(Mn);

    // Setup is running in the loop area to handle exceptions...
    IF_MODULE_ERROR(Mn)
    {
        CLEAR_MODULE_ERRORS(Mn);  // do nothing, clear flags
    }
    // Setup is running in the setup area following power on
    else
    {
        UI_16 TestReturn = 0;
        TestReturn = ExceptionsTest(MODdataPTR(Mn));
        MODdataPTR(Mn)->ExceptionsTestReturn = TestReturn;
        
        // Basic Platfrom Serialization / Deserialization if Included        
        #ifdef __USINGCONSOLEMENU            
            TestReturn = SerializationTest(MODdataPTR(Mn));            
            MODdataPTR(Mn)->SerializationTestReturn = TestReturn; 
        #endif
        
        // initialize values for timedexecutiontest in loop
        MODdataPTR(Mn)->ui32_0 = 0;
        MODdataPTR(Mn)->ui32_1 = 0;
        MODdataPTR(Mn)->ui16_0 = 0;
        MODdataPTR(Mn)->ui16_1 = 0;
    }
    return RETURN_SUCCESS;
}

// Re-usable, portable, cross-platform (ccNOosTests loop() function)
MODdeclareLOOP(Mn)
{
    MODDATAPTR_ERROR_RETURN(Mn);
    
    UI_16 TestReturn = 0;
    switch(MODdataPTR(Mn)->TestState)
    {
        case 0x0000:// Timed Execution Loop Test
            TestReturn = TimedExecutionTest(MODdataPTR(Mn));
            MODdataPTR(Mn)->TimedExecutionTestReturn = TestReturn;
            if(TestReturn!=RETURN_TEST_IN_PROGRESS)
                MODdataPTR(Mn)->TestState++;
            break;
        case 0x0001:// Exception Loop Test
            TestReturn = ExceptionsTest(MODdataPTR(Mn));
            MODdataPTR(Mn)->ExceptionsTestReturn = TestReturn;
            if(TestReturn!=RETURN_TEST_IN_PROGRESS)
                MODdataPTR(Mn)->TestState++;
            break;
        default:// Done!!!
            if( MODdataPTR(Mn)->SerializationTestReturn==RETURN_TEST_PASSED 
             && MODdataPTR(Mn)->TimedExecutionTestReturn==RETURN_TEST_PASSED
             && MODdataPTR(Mn)->ExceptionsTestReturn==RETURN_TEST_PASSED)
                MODdataPTR(Mn)->TestState = 0xffff;
            else
                MODdataPTR(Mn)->TestState = 0xfffe;
            break;
    }
    
    MODprintMENU(Mn)(compModPtrIn);

    return RETURN_SUCCESS;
}

MODdeclareSYSTICK(Mn) { ; }  // do nothing in the systick area

#ifdef __USINGCONSOLEMENU

MODdeclarePRINTm(Mn)
{
    MODDATAPTR_ERROR_RETURN(Mn);

    INIT_MENU_VARS(charBuffMax, MODdataPTR(Mn)->charbuff_Out);
    // looping and printing
    // for looping...
    int lines2Print = 1;
    int linesPrinted = 0;
    if (MODdataPTR(Mn)->charsRead > 0)
    {
        MODdataPTR(Mn)->charsRead = 0;
        while (lines2Print > 0)
        {
            switch (linesPrinted)
            {
            case 0:
                PRINT_MENU_LN  "\033[2J\033[0;0H\n////////// Console Menu - ccNOos Tests"     END_MENU_LN;
            case 1:
                PRINT_MENU_LN  "\nStatus - ccNOos Tests:\t%s", StatusccNOosTests(MODdataPTR(Mn))      END_MENU_LN;
            case 2:
                PRINT_MENU_LN  "\nResults - Serialization Tests:\t%s", ResultsSerializationTests(MODdataPTR(Mn))      END_MENU_LN;
            case 3:
                PRINT_MENU_LN  "\nResults - Timed Execution Tests:\t%s", ResultsTimedExecutionTests(MODdataPTR(Mn))      END_MENU_LN;
            case 4:
                PRINT_MENU_LN  "\n//////////////////////////////////"      END_MENU_LN;
            case 5:
                PRINT_MENU_LN  "\nType \"ccNOosTests:Tx:22.75;\" set Tx to 22.75"      END_MENU_LN;
            case 6:
                PRINT_MENU_LN  "\nType \"ccNOosTests:Rx:21.00;\" set Rx to 21.00"      END_MENU_LN;
            case 7:
                PRINT_MENU_LN  "\nType \"ccNOosTests:Xx:19.25;\" set Xx to 19.25"      END_MENU_LN;
            case 8:
                PRINT_MENU_LN "\nInput>>" END_MENU_LN;
            default:
                lines2Print = 0;
                break;
            }

            MODdataPTR(Mn)->chars2Write = charsWritten;
            linesPrinted++;

            if (lines2Print > 0)
                WriteMenuLine(&MODdataPTR(Mn)->charbuff_Out[0]);
        }
    }
    return charsWritten;
}

MODdeclarePARSEi(Mn)
{
    MODDATAPTR_ERROR_RETURN(Mn);
}
const char* ResultsTimedExecutionTests(MODdeclarePTRIN(Mn))
{
    if(MODdataPTR(Mn)->TimedExecutionTestReturn==RETURN_TEST_PASSED)
        return "Passed!";
    else if(MODdataPTR(Mn)->TimedExecutionTestReturn==RETURN_TEST_IN_PROGRESS)
        return "Running...";
    else
        return "Failed";
}
const char* StatusccNOosTests(MODdeclarePTRIN(Mn))
{
    if(MODdataPTR(Mn)->TestState==0x0000)
        return "Not Running";
    else if(MODdataPTR(Mn)->TestState==0xffff)
        return "Passed!";
    else if(MODdataPTR(Mn)->TestState==0xfffe)
        return "Failed - :(";
    else
        return "Running...";
}
const char* ResultsSerializationTests(MODdeclarePTRIN(Mn))
{
    if(MODdataPTR(Mn)->SerializationTestReturn==RETURN_TEST_PASSED)
        return "Passed!";
    else if(MODdataPTR(Mn)->SerializationTestReturn==RETURN_FAILED_SERIALIZATION)
        return "Failed - Serialization";
    else if(MODdataPTR(Mn)->SerializationTestReturn==RETURN_FAILED_DESERIALIZATION)
        return "Failed - Deserialization";
    else if(MODdataPTR(Mn)->SerializationTestReturn==RETURN_FAILED_COMPARISON)
        return "Failed - Comparison";
    else
        return "Status - Unknown";
}
UI_16 SerializationTest(MODdeclarePTRIN(Mn))
{
    int bytesWritten = 0; 
    UI_8 parseResults = 0;
    
    #define SerializeTestPattern(VAR, VAL, FORMAT, LEN, FUNC) MODdataPTR(Mn)->VAR##0 = VAL;\
    bytesWritten = SN_PrintF(MODdataPTR(Mn)->charbuff_Out,LEN,FORMAT,MODdataPTR(Mn)->VAR##0);\
    if( bytesWritten != LEN || \
        ui8TRUE!=stringMatchCaseSensitive(MODdataPTR(Mn)->charbuff_Out,xstr(VAL))\
        )\
        return RETURN_FAILED_SERIALIZATION;\
    stringInit(MODdataPTR(Mn)->charbuff_In, xstr(VAL));\
    parseResults = FUNC(MODdataPTR(Mn)->charbuff_In, &MODdataPTR(Mn)->VAR##1 );\
    if(parseResults != ui8TRUE)\
        return RETURN_FAILED_DESERIALIZATION;\
    if(MODdataPTR(Mn)->VAR##0 != MODdataPTR(Mn)->VAR##1)\
        return RETURN_FAILED_COMPARISON;
    
    SerializeTestPattern(double_, 3.1457, "%6.4f", 6, ATO_D);    
    SerializeTestPattern(float_, -3.1457, "%6.4f", 7, ATO_F);
    
    SerializeTestPattern(ui64_, 101001987, "%d", 9, ATO_U64);
    SerializeTestPattern(i64_, -101001987, "%d", 9, ATO_I64);

    SerializeTestPattern(ui32_, 65409, "%u", 9, ATO_U32);
    SerializeTestPattern(i32_, -65409, "%d", 10, ATO_I32);
    
    SerializeTestPattern(ui16_, 0x0bab, "0x%x", 6, ATO_U16);
    SerializeTestPattern(i16_, -2987, "%i", 5, ATO_I16);
    
    SerializeTestPattern(ui8_, 0x0F, "0x%X", 4, ATO_U8);
    SerializeTestPattern(i8_, -15, "%i", 3, ATO_I8);
    
    return RETURN_TEST_PASSED;
    
    #undef SerializeTestPattern
}
#endif // !__USINGCONSOLEMENU
UI_16 TimedExecutionTest(MODdeclarePTRIN(Mn))
{
    #define uSNOW MODdataPTR(Mn)->ui32_0
    #define uSTHEN MODdataPTR(Mn)->ui32_1
    #define LOOPCYCLES MODdataPTR(Mn)->ui16_0

    uSNOW = getuSecTicks();
    if(LOOPCYCLES++ > 0 && uSNOW != uSTHEN)
    {
        if(LOOPCYCLES > 1 && (uSNOW-uSTHEN) >= getuSecPerSysTick())
        {
            return RETURN_TEST_PASSED;
        }
        else if(uSNOW>uSTHEN)
        {
            uSTHEN = uSNOW;
            return RETURN_TEST_IN_PROGRESS;
        }
        else
            return RETURN_FAILED_TIMEDEXECUTION;
    }
    else if(LOOPCYCLES > 1)
    {
        return RETURN_FAILED_TIMEDEXECUTION;
    }
    else if(LOOPCYCLES == 0)
        return RETURN_TEST_IN_PROGRESS;

    // should never get here, only in place to prevent compiler warning
    return RETURN_FAILED_TIMEDEXECUTION;

    #undef uSNOW
    #undef uSTHEN
    #undef LOOPCYCLES
}
UI_16 ExceptionsTest(MODdeclarePTRIN(Mn))
{

    return RETURN_TEST_PASSED;
}

#endif // !COMPILE_TESTS
