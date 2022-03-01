/** \file console_menu.h
    \brief ccNOos Console Menu Definitions

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
#ifdef __USINGCONSOLEMENU
    
#include "console_menu.h"
#include "execution_system.h"

const char* cursorString(UI_8 showCursor)
{
    if (showCursor == ui8TRUE)
        return "->";
    else
        return "  ";
}
const char* terminalClearString()
{
    return "\033[2J\033[0;0H";
}
const char* terminalSlashes()
{
    return "\\\\\\\\\\";
}

// Console UI Data Structure
struct uiStruct createuiStruct()
{
    struct uiStruct outStruct;
    outStruct.currentMenuIndex = 0;
    outStruct.currentUserLevel = 0;
    outStruct.devptr = nullptr;
    outStruct.lines2print = 0;
    outStruct.linesprinted = 0;
    outStruct.clearScreen = ui8FALSE;
    outStruct.parseIndex = 0;
    outStruct.showHelp = ui8FALSE;
    outStruct.showPrompt = ui8FALSE;
    return outStruct;
}


// Logging Interface Data Structure
struct logStruct createlogStruct()
{
    struct logStruct outStruct;
    outStruct.devptr = nullptr;
    return outStruct;
}


// Config Interface Data Structure
struct configStruct createconfigStruct()
{
    struct configStruct outStruct;
    outStruct.devptr = nullptr;
    return outStruct;
}




UI_8 isASCIIchar(char inChar) { return ((inChar >= ASCII_space && inChar <= ASCII_tilda) || inChar == ASCII_lf || inChar == ASCII_cr || inChar == ASCII_tab || inChar == 0x00); }
UI_8 isLetterchar(char inChar) { return ((inChar >= ASCII_A && inChar <= ASCII_Z) || (inChar >= ASCII_a && inChar <= ASCII_z)); }
UI_8 isNumberchar(char inChar) { return ((inChar >= ASCII_0 && inChar <= ASCII_9) || inChar == ASCII_plus || inChar == ASCII_minus || inChar == ASCII_dot); }
UI_8 isIntegerchar(char inChar) { return ((inChar >= ASCII_0 && inChar <= ASCII_9) || inChar == ASCII_plus || inChar == ASCII_minus); }
UI_8 isUnsignedIntegerchar(char inChar) { return ((inChar >= ASCII_0 && inChar <= ASCII_9)); }
UI_8 isDelimiterchar(char inChar) { return (inChar == ASCII_colon); }
UI_8 isTerminatorchar(char inChar) { return (inChar == ASCII_semicolon); }
UI_8 isASCIIString(char* inStringPtr) { int index = 0;  while (inStringPtr[index] != 0x00) if (!isASCIIchar(inStringPtr[index++])) return ui8FALSE; return ui8TRUE; }
UI_8 isLetterString(char* inStringPtr) { int index = 0;  while (inStringPtr[index] != 0x00) if (!isLetterchar(inStringPtr[index++])) return ui8FALSE; return ui8TRUE; }
UI_8 isNumberString(char* inStringPtr) { int index = 0;  while (inStringPtr[index] != 0x00) if (!isNumberchar(inStringPtr[index++])) return ui8FALSE; return ui8TRUE; }
UI_8 isIntegerString(char* inStringPtr) { int index = 0;  while (inStringPtr[index] != 0x00) if (!isIntegerchar(inStringPtr[index++])) return ui8FALSE; return ui8TRUE; }
UI_8 isUnsignedIntegerString(char* inStringPtr) { int index = 0;  while (inStringPtr[index] != 0x00) if (!isUnsignedIntegerchar(inStringPtr[index++])) return ui8FALSE; return ui8TRUE; }
UI_8 stringMatchCaseSensitive(char* inStringPtr, const char* matchString) 
{ 
    int i = 0; 
    while (matchString[i] != 0x00) 
    { 
        if (inStringPtr[i] != matchString[i]) 
            return ui8FALSE; 
        i++; 
    } 
    return ui8TRUE; 
}
void stringInit(char* stringPtr, const char* initString)
{
    int idx = -1;
    do {
        idx++;
        stringPtr[idx] = initString[idx];
    } while (initString[idx] != 0x00);
}
UI_16 stringLength(const char* stringPtr)
{
    UI_16 lenOut = 0;
    while (lenOut < charBuffMax) {
        if (stringPtr[lenOut] == 0x00)
            break;
        lenOut++;
    }
    return lenOut;

}
#endif

