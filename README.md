# ccNOos - Build Portable Microcontroller Applications!
- Straight C for any Microcontroller ex. [PSoC4](https://www.infineon.com/cms/en/product/evaluation-boards/cy8ckit-042/?utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-dev_kit), [dsPIC33f](https://www.microchip.com/en-us/development-tool/DM330013)
- Straight C for OS [Kernel](https://www.kernel.org/doc/html/v4.13/process/howto.html) and [Driver](https://www.kernel.org/doc/html/v4.11/driver-api/index.html) Development
- C++ Wrappers for any Microcontroller ex. [Arduino](https://www.arduino.cc/en/Main/arduinoBoardMega2560), [PICMZ32](https://www.microchip.com/en-us/development-tool/dm320007) 
- [Straight C Libraries reuasable anywhere](https://github.com/InMechaSol/ccNOos/tree/main/ccLibs)
- Building Blocks for Application Development
  - [Execution System](https://github.com/InMechaSol/ccNOos/tree/main/executionSystem)
    - API to both [Platform Specific Main](https://github.com/InMechaSol/ccNOos/tree/main/tests/testMainTemplates) and [Portable Application Compute Modules](https://github.com/InMechaSol/ccNOos/tree/main/tests/testApps)  
- C/C++ [Tests](https://github.com/InMechaSol/ccNOos/tree/main/tests/testApps/ccNOos_Tests)
- C/C++ [Example Applications](https://github.com/InMechaSol/ccNOos/tree/main/tests/testApps/SysTick)
### ccNOos Application Features
- [Cross-Platform Applications with compile-time selectable](https://github.com/InMechaSol/ccNOos/tree/main/tests/testApps)
  - serialization and 
  - platform specification
- [Tested, Verified, Gauranteed Performance Across Platforms](https://github.com/InMechaSol/ccNOos_Tests)
- [Reusable Platform Specification](https://github.com/InMechaSol/ccNOos/tree/main/tests/testPlatforms)
  - shared acrros all applications of a particular platform  
- [Common, compiler configured, Platform Specific Main file](https://github.com/InMechaSol/ccNOos/tree/main/tests/testMainTemplates)
  - shared accross all platforms for a particular application
## ccNOos Application Structure
![appStructure](/docs/pngs/Application_Structure.png)

