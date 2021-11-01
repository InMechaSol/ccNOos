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
#include "console_menu.h"
#include "../executionSystem/execution_system.h"

struct consoleMenuStruct CreateConsoleMenuStruct()
{
    struct consoleMenuStruct outStruct;
    return outStruct;
}