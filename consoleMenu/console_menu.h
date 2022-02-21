/** \file console_menu.h
    \brief ccNOos Console Menu Declarations 

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

A console menu provides a funtions for rendering the menu text.  
A console menu provides functions for text input parsing and button/key press actions.
A console menu has configurations for different viewing sizes (ssh terminal, small lcd and key pad, large lcd and key pad)

b/c its an API Module...
A console menu has access restrictions based on user access level.
A console menu has data access to the full execution system and all it contains.
    

*/
#ifndef __CONSOLE_MENU__
#define __CONSOLE_MENU__


#include "compute_module.h"
#include "io_device.h"

#ifdef __USINGCONSOLEMENU

// Console UI Data Structure
struct uiStruct
{
    UI_8    charsRead, chars2Write;
    char    charbuff_In[charBuffMax];
    char    charbuff_Out[charBuffMax];
};

struct menuNode
{
    int (*PrintMenuScreen)(struct computeModuleStruct* CompModPtrIn);
    int (*ParseInputString)(struct computeModuleStruct* CompModPtrIn);
};

struct consoleMenuStruct // declaration of execution system data structure
{
    char* inputBufferPtr;
    int inputBufferSize;
    char* outputBufferPtr;
    int outputBufferSize;
    struct menuNode* rootNode;
    struct menuNode* visibleNode;
};

struct consoleMenuStruct CreateConsoleMenuStruct(
        char* inputBufferPtrIn,
        int inputBufferSizeIn,
        char* outputBufferPtrIn,
        int outputBufferSizeIn,
        struct menuNode* rootNodeIn
        );

// String-ize macros
#define _TOSTRING(s) #s
#define TOSTRING(s) _TOSTRING(s)
// ASCII Constants for Communication Interfaces
#define ASCII_space             32
#define ASCII_A                 65
#define ASCII_Z                 90
#define ASCII_a                 97
#define ASCII_z                 122
#define ASCII_0                 48
#define ASCII_9                 57
#define ASCII_plus              43
#define ASCII_minus             45
#define ASCII_dot               46
#define ASCII_colon             58
#define ASCII_semicolon         59
#define ASCII_tilda             126
#define ASCII_lf				10
#define ASCII_cr				13
#define ASCII_tab				9

#define __INIT_MENU_VARS(CHARS_PER_LINE, CHAR_PTR)  \
        char* linebuff = CHAR_PTR;\
        int charsWritten = 0;\
        int maxChars = CHARS_PER_LINE
#define INIT_MENU_VARS(CHARS_PER_LINE, CHAR_PTR) __INIT_MENU_VARS(CHARS_PER_LINE, CHAR_PTR)

#define __PRINT_MENU_LN SN_PrintF( linebuff, maxChars, 
#define PRINT_MENU_LN __PRINT_MENU_LN

#define __END_MENU_LN ); break
#define END_MENU_LN __END_MENU_LN


// when possible, simply return snprintf() from std lib

int SN_PrintF(char* str, unsigned int size, const char* format, ...);
UI_8 ATO_F(const char* str, float* val);
UI_8 ATO_D(const char* str, double* val);

UI_8 ATO_I8(const char* str, I_8* val);
UI_8 ATO_I16(const char* str, I_16* val);
UI_8 ATO_I32(const char* str, I_32* val);
UI_8 ATO_I64(const char* str, I_64* val);
UI_8 ATO_U8(const char* str, UI_8* val);
UI_8 ATO_U16(const char* str, UI_16* val);
UI_8 ATO_U32(const char* str, UI_32* val);
UI_8 ATO_U64(const char* str, UI_64* val);


UI_8 isASCIIchar(char inChar);
UI_8 isLetterchar(char inChar);
UI_8 isNumberchar(char inChar);
UI_8 isIntegerchar(char inChar);
UI_8 isUnsignedIntegerchar(char inChar);
UI_8 isDelimiterchar(char inChar);
UI_8 isTerminatorchar(char inChar);
UI_8 isASCIIString(char* inStringPtr);
UI_8 isLetterString(char* inStringPtr);
UI_8 isNumberString(char* inStringPtr);
UI_8 isIntegerString(char* inStringPtr);
UI_8 isUnsignedIntegerString(char* inStringPtr);
UI_8 stringMatchCaseSensitive(char* inStringPtr, const char* matchString);
void stringInit(char* stringPtr, const char* initString);
    
void WriteMenuLine(char* outStringPtr); // rely on 0x00 termination? safer with length parameter
void GetMenuChars(char* inStringPtr);

void WriteLogLine(char* outStringPtr);
void ReadConfigLine(char* inStringPtr);

#define PlatformAPIFuncsTemplate(SizePlusOne) \
int SN_PrintF(char* str, unsigned int size, const char* format, ...)\
{\
    va_list argptr;\
    va_start(argptr, format);\
    int chars = vsnprintf(str, SizePlusOne, format, argptr);\
    va_end(argptr);\
    return chars;\
}\
UI_8 ATO_F(const char* str, float* val)\
{\
    if (isNumberString((char*)str))\
    {\
        *val = (float)atof(str);\
        return ui8TRUE;\
    }\
    else\
        return ui8FALSE;\
}\
UI_8 ATO_D(const char* str, double* val)\
{\
    if (isNumberString((char*)str))\
    {\
        *val = atof(str);\
        return ui8TRUE;\
    }\
    else\
        return ui8FALSE;\
}\
UI_8 ATO_I8(const char* str, I_8* val)\
{\
    if (isIntegerString((char*)str))\
    {\
        *val = (I_8)atoi(str);\
        return ui8TRUE;\
    }\
    else\
        return ui8FALSE;\
}\
UI_8 ATO_I16(const char* str, I_16* val)\
{\
    if (isIntegerString((char*)str))\
    {\
        *val = (I_16)atoi(str);\
        return ui8TRUE;\
    }\
    else\
        return ui8FALSE;\
}\
UI_8 ATO_I32(const char* str, I_32* val)\
{\
    if (isIntegerString((char*)str))\
    {\
        *val = (I_32)atol(str);\
        return ui8TRUE;\
    }\
    else\
        return ui8FALSE;\
}\
UI_8 ATO_I64(const char* str, I_64* val)\
{\
    if (isIntegerString((char*)str))\
    {\
        *val = (I_64)atol(str);\
        return ui8TRUE;\
    }\
    else\
        return ui8FALSE;\
}\
UI_8 ATO_U8(const char* str, UI_8* val)\
{\
    if (isUnsignedIntegerString((char*)str))\
    {\
        *val = (UI_8)atol(str);\
        return ui8TRUE;\
    }\
    else\
        return ui8FALSE;\
}\
UI_8 ATO_U16(const char* str, UI_16* val)\
{\
    if (isUnsignedIntegerString((char*)str))\
    {\
        *val = (UI_16)atol(str);\
        return ui8TRUE;\
    }\
    else\
        return ui8FALSE;\
}\
UI_8 ATO_U32(const char* str, UI_32* val)\
{\
    if (isUnsignedIntegerString((char*)str))\
    {\
        *val = (UI_32)atol(str);\
        return ui8TRUE;\
    }\
    else\
        return ui8FALSE;\
}\
UI_8 ATO_U64(const char* str, UI_64* val)\
{\
    if (isUnsignedIntegerString((char*)str))\
    {\
        *val = (UI_64)atol(str);\
        return ui8TRUE;\
    }\
    else\
        return ui8FALSE;\
}



#ifdef __cplusplus



class consoleMenuClass // declaration of console menu class
{

};


#endif // !__cplusplus
#endif // !__USINGCONSOLEMENU
#endif // ! __CONSOLE_MENU__
