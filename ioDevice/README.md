# ioDevice - Abstract Access to Real Hardware
- [Compute Modules](https://github.com/InMechaSol/ccNOos/tree/main/computeModule) are written to work with Abstract ioDevices
- The [Execution System](https://github.com/InMechaSol/ccNOos/tree/main/executionSystem) is aware of all ioDevices on a Platform
- Linking between Platform ioDevices, the [Execution System](https://github.com/InMechaSol/ccNOos/tree/main/executionSystem), and [Cross-Platform Application Source](https://github.com/InMechaSol/ccNOos/tree/main/tests/testApps/SysTick) is done in
  - [Platform Specification Files](https://github.com/InMechaSol/ccNOos/tree/main/tests/testPlatforms) and
  - [Platform Specific Application Main Files](https://github.com/InMechaSol/ccNOos/tree/main/tests/testMainTemplates) 
