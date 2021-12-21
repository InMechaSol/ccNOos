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

#include "../computeModule/compute_module.h"
#include "../ioDevice/io_device.h"



struct consoleMenuStruct // declaration of execution system data structure
{
    char* inputBufferPtr;
    int inputBufferSize;
    char* outputBufferPtr;
    int outputBufferSize;
};

struct consoleMenuStruct CreateConsoleMenuStruct(
        char* inputBufferPtrIn,
        int inputBufferSizeIn,
        char* outputBufferPtrIn,
        int outputBufferSizeIn);

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
        int startChar = 0;\
        int maxChars = CHARS_PER_LINE
#define INIT_MENU_VARS(CHARS_PER_LINE, CHAR_PTR) __INIT_MENU_VARS(CHARS_PER_LINE, CHAR_PTR)

#define __PRINT_MENU_LN SN_PrintF( linebuff, maxChars, 
#define PRINT_MENU_LN __PRINT_MENU_LN

#define __END_MENU_LN ); break
#define END_MENU_LN __END_MENU_LN


// when possible, simply return snprintf() from std lib
int SN_PrintF(char* str, unsigned int size, const char* format, ...);
bool ATO_F(const char* str, float* val);
bool ATO_D(const char* str, double* val);
bool ATO_I8(const char* str, int8_t* val);
bool ATO_I16(const char* str, int16_t* val);
bool ATO_I32(const char* str, int32_t* val);
bool ATO_I64(const char* str, int64_t* val);
bool ATO_U8(const char* str, uint8_t* val);
bool ATO_U16(const char* str, uint16_t* val);
bool ATO_U32(const char* str, uint32_t* val);
bool ATO_U64(const char* str, uint64_t* val);


bool isASCIIchar(char inChar);
bool isLetterchar(char inChar);
bool isNumberchar(char inChar);
bool isIntegerchar(char inChar);
bool isUnsignedIntegerchar(char inChar);
bool isDelimiterchar(char inChar);
bool isTerminatorchar(char inChar);
bool isASCIIString(char* inStringPtr);
bool isLetterString(char* inStringPtr);
bool isNumberString(char* inStringPtr);
bool isIntegerString(char* inStringPtr);
bool isUnsignedIntegerString(char* inStringPtr);
bool stringMatchCaseSensitive(char* inStringPtr, const char* matchString);

void WriteMenuLine(char* outStringPtr); // rely on 0x00 termination? safer with length parameter
void GetMenuChars(char* inStringPtr);

void WriteLogLine(char* outStringPtr);
void ReadConfigLine(char* inStringPtr);

#ifdef __cplusplus



class consoleMenuClass // declaration of console menu class
{

};


#endif // !__cplusplus
#endif // ! __CONSOLE_MENU__
